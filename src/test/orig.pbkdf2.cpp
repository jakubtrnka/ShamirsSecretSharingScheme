/* Jakub Trnka June 2018
 * from https://www.emc.com/collateral/white-papers/h11302-pkcs5v2-1-password-based-cryptography-standard-wp.pdf
 ***************************************************************************************************************
 * pkcs5v2-1-PBKDF2(P, S, c, keylen)
 * P...password
 * S...salt
 * c...iter count
 * ******************************************
 * T1 = F(P, S, c, 1)
 * T2 = F(P, S, c, 2)
 * ...
 * Tn = F(P, S, c, n)
 * ******************************************
 * F(P, S, c, i) = U1 xor U2 xor ... xor Uc
 * ******************************************
 * U1 = PRF(key = P, text = S|be(i))      be... Big-endian int32
 * U2 = PRF(key = P, text = U1)
 * U3 = PRF(key = P, text = U2)
 * ...
 * Uc = PRF(P, U(c-1))
 *
 * first argument to PRF ... (hmac) key  (=password)
 * second argument to PRF... (hmac) text (=salt)
 * ******************************************
 * key = strip-to-keylen(T1 || T2 || ... Tm)      
 ***********************************************************************
 *
 * from SLIP0039 document:
 * PBKDF2(
 * 	PRF = HMAC-SHA256,
 * 	Password = master_secret,
 * 	Salt = "SLIP0039" + passphrase,
 * 	iterations = 20000,
 * 	dkLen = 256 bits (i.e. 32 Bytes)
 * 	)
 *
 */
#include <pbkdf2.h>
#include <hmac_sha256.h>
#include <hmac_sha512.h>

#include <endian.h>


namespace {
	union overflower {
		uint32_t number;
		uint8_t array[4];
	};

	void u_hmac_sha256(std::vector<uint8_t> & u_state, const std::vector<uint8_t> & master_secret) {
		CHMAC_SHA256(master_secret.data(), master_secret.size()).Write(u_state.data(), u_state.size()).Finalize(u_state.data());
	}

	void f_hmac_sha256(std::vector<uint8_t> & output, const std::vector<uint8_t> & master_secret, const std::string & passphrase, int iter_count, uint32_t index) {
		std::vector<uint8_t> u_state(passphrase.begin(), passphrase.end());
		overflower iter_octet;
		uint8_t hmc[32];
		iter_octet.number = htobe32(index);
		for (int i=0; i<4; ++i) u_state.push_back(iter_octet.array[i]);

		/* CHMAC_SHA256( hmac_key, hmac_keylen) . Write(hmac_text, hmac_textlen) . Finalize(u1)*/
		CHMAC_SHA256(master_secret.data(), master_secret.size()) . Write(u_state.data(), u_state.size()) . Finalize(hmc);
		u_state.resize(32);
		for (int i=0; i<32; ++i) u_state.at(i) = hmc[i];

		std::vector<uint8_t> t_hmac_sha256(u_state);
		while (--iter_count > 0) {
			u_hmac_sha256(u_state, master_secret); 
			for (int i=0; i<32; ++i) t_hmac_sha256.at(i) ^= u_state.at(i);
		}
		for (int i=0; i<32; ++i) output.push_back(t_hmac_sha256.at(i));
	}

	/* native PKCS5 PBKDF2 */
	std::vector<uint8_t> pbkdf2_hmac_sha256(const std::vector<uint8_t> & master_secret, const std::string & passphrase, int iter_count, size_t key_len) {
		std::vector<uint8_t> output;
		uint32_t octets_generated(0), i(0);
		while (octets_generated < key_len) {
			f_hmac_sha256(output, master_secret, passphrase, iter_count, ++i);
			octets_generated += 32;
		}
		output.resize(key_len);
		return output;
	}
} // anonymous namespace

namespace Shamir {
	std::vector<uint8_t> slip39_pbkdf2(const std::vector<uint8_t> & secret, const std::string passphrase) {
		return pbkdf2_hmac_sha256(secret, std::string("SLIP0039")+passphrase, 20000, 32);
	}
} // Shamir namespace
