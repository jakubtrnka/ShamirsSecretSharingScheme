#include <wordlist.h>
#include <sha256.h>

#include <algorithm>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

namespace Shamir {
	std::vector<uint8_t> & append_bip39_checksum(std::vector<uint8_t> & it) {
		CSHA256 h;
		uint8_t hash[32];
		h.Write(it.data(), it.size());
		h.Finalize(hash);
		int seedchunks = it.size() / 4;
		for (auto i=0; i < seedchunks/8; ++i) it.push_back(hash[i]);
		if (seedchunks % 8 != 0) it.push_back(hash[seedchunks/8] & (0xff << (8 - seedchunks % 8)));
		return it;
	}

	std::vector<uint8_t> & check_bip39_checksum(std::vector<uint8_t> & it) {
		auto databitlen = it.size() * 8;
		databitlen -= databitlen % 33;
		if (databitlen % 8 != 0) {  // zero-pad last block (byte) of the data
			it.back() &= (0xff << (8 - (databitlen % 8)));
		}
		auto checksumsize = databitlen - (databitlen/33)*32;
		CSHA256 h;
		uint8_t hash[32];
		h.Write(it.data(), (databitlen/33)*4);
		h.Finalize(hash);
		if (checksumsize % 8 != 0) hash[checksumsize / 8] &= (0xff << (8 - (checksumsize % 8)));
		for (auto i=0u; i < checksumsize/8 + (checksumsize % 8 ==0? 0 : 1); ++i) { // TODO: clean this thing
			if (it[(databitlen/33)*4 + i] != hash[i]) throw "BIP39 checksum verification failed";
		}
		it.resize((databitlen/33)*4);
		return it;
	}

	/** Converts vector of integers representing number base 2^p to a byte-vector
	  * with complexity O( vector.size() )
	  * power of 2 in a base must be 9 to 24
	  */
	std::vector<uint8_t> power2ToHex(const std::vector<int> & it, int p) {
		if ( p < 9 || p > 24 ) throw "base 2-power must be between 9 and 24";
		std::vector<uint8_t> output;
		size_t appended(0);
		for (auto &x: it) {
			int lastleft = (8 - appended % 8) % 8;
			int toappend = p;
			if (lastleft != 0) {
				output.back() |= static_cast<uint8_t>(x >> (p - lastleft));
				toappend -= lastleft;
				appended += lastleft;
			}
			while (toappend >= 8) {
				output.push_back(static_cast<uint8_t>(x >> (toappend - 8)));
				toappend -= 8;
				appended += 8;
			}
			if (toappend != 0) {
				output.push_back(static_cast<uint8_t>(x << (8 - toappend)));
				appended += toappend;
			}
		}
		return output;
	}
	
	/** Converts vector of bytes into array of integers representing number base 2^p
	 * with complexity O( vector.size() )
	 * power of 2 in a base must be 9 to 24
	 */
	std::vector<int> hexToPower2(const std::vector<uint8_t> & data, int p) {
		//std::cout << "olala " << data.size() << "\n";
		if ( p < 9 || p > 24 ) throw "base 2-power must be between 9 and 24";
		std::vector<int> output;
		uint32_t bitholder(0);
		int bitsread(0);
		for (auto & x: data) {
			int willread = (((p - bitsread) > 8) ? 8 : (p - bitsread));
			bitholder <<= willread;
			bitholder |= (x >> (8 - willread));
			bitsread += willread;
			if (bitsread == p) {
				output.push_back(bitholder);
				bitholder = x & (0xff >> (willread));
				bitsread = 8 - willread;
			}
		}
		int last = bitholder << (p - bitsread);
		//if (bitsread != 0) output.push_back(last);
		//if (last != 0) output.push_back(last);
		output.push_back(last);
		return output;
	}

	std::vector<int> slip39ToNum(const std::vector<std::string> & in) {
		std::vector<int> output;
		output.reserve(in.size());
		for (auto & ii: in) {
			auto element = std::lower_bound(slip_words.data(), slip_words.data() + slip_words.size(), ii);
			if (*element != ii) throw "Given word not in SLIP39 dictionary\n";
			output.push_back(static_cast<int>(element - slip_words.data()));
		}
		return output;
	}

	std::vector<int> bip39ToNum(const std::vector<std::string> & in) {
		std::vector<int> output;
		output.reserve(in.size());
		for (auto & ii: in) {
			auto element = std::lower_bound(bip_words.data(), bip_words.data() + bip_words.size(), ii);
			if (*element != ii) throw "Given word not in BIP39 dictionary\n";
			output.push_back(static_cast<int>(element - bip_words.data()));
		}
		return output;
	}
	
} // Shamir namespace

