#include <slip39_wrapper.h>
#include <pbkdf2.h>
#include <wordlist.h>
#include <share.h>
#include <bit_container.h>
#include <get_insecure_randomness.h>

namespace Shamir {
	std::vector<std::vector<std::string>> fromEnthropy(const std::vector<uint8_t> & enthropy, uint16_t count, uint16_t threshold) {
		auto shares = distribute_secret_slip(enthropy, count, threshold);
		std::vector<std::vector<std::string>> output(shares.size());
		for (unsigned i=0; i < shares.size(); ++i) {
			auto line = hexToPower2(shares.at(i), 10);
			auto linebits = (enthropy.size()*8 + 42);
			line.resize(linebits / 10 + ((linebits % 10) ? 1 : 0));
			for (auto word: line) output.at(i).push_back(slip_words[word]);
		}
		return output;
	}

	std::vector<std::vector<std::string>> randEnthropy(int bits, uint16_t count, uint16_t threshold, const std::string & password) {
		std::vector<uint8_t> enthropy(bits/8);
		pseudo_random_fill(enthropy);
		return fromEnthropy(enthropy, count, threshold);
	}

	std::vector<uint8_t> toEnthropy(const std::vector<std::vector<std::string>> & mnemonics) {
		std::vector<std::vector<uint8_t>> decoded_shares;
		for (unsigned i=0; i<mnemonics.size(); ++i) {
			auto line = slip39ToNum(mnemonics.at(i));
			auto hex_line = power2ToHex(line, 10);
			decoded_shares.push_back(hex_line);
		}
		return reconstruct_secret_slip(decoded_shares);
	}

	std::vector<uint8_t> toSeed(const std::vector<std::vector<std::string>> & mnemonics, const std::string & password) {
		return slip39_pbkdf2(toEnthropy(mnemonics), password);
	}
} // Shamir
