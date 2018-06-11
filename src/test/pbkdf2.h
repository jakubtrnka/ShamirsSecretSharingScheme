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


#ifndef PBKDF2_H
#define PBKDF2_H 1

#include <vector>
#include <string>
#include <cstdint>

namespace Shamir {
	std::vector<uint8_t> slip39_pbkdf2(const std::vector<uint8_t> & secret, const std::string passphrase);
#ifdef BIP39
	std::vector<uint8_t> bip39_pbkdf2(const std::vector<uint8_t> & secret, const std::string passphrase);
#endif
}




#endif
