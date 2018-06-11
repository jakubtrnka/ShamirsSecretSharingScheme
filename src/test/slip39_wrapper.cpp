#include <slip39_wrapper.h>
#include <pbkdf2.h>
#include <wordlist.h>
#include <share.h>
#include <bit_container.h>
#include <get_insecure_randomness.h>

namespace Shamir {

	std::vector<std::vector<std::string>> fromEnthropy(const std::vector<uint8_t> & enthropy, uint16_t count, uint16_t threshold, const std::string & password) {
		auto shares = distribute_secret_slip(enthropy, count, threshold);
		std::vector<std::vector<std::string>> output(shares.size());
		for (unsigned i=0; i < shares.size(); ++i) {
			auto line = hexToPower2(shares.at(i), 10);
			for (auto word: line) output.at(i).push_back(slip_words[word]);
		}
		return output;
	}
	
	std::vector<std::vector<std::string>> randEnthropy(int bits, uint16_t count, uint16_t threshold, const std::string & password) {
		std::vector<uint8_t> enthropy(bits/8);
		pseudo_random_fill(enthropy);
		return fromEnthropy(enthropy, count, threshold, password);
	}
	
	std::vector<uint8_t> toEnthropy(const std::vector<std::vector<std::string>> & mnemonics, const std::string & password) {
		std::vector<std::vector<uint8_t>> decoded_shares;
		for (unsigned i=0; i<mnemonics.size(); ++i) {
			auto line = slip39ToNum(mnemonics.at(i));
			decoded_shares.push_back(power2ToHex(line, 10));
		}
		return slip39_pbkdf2(reconstruct_secret_slip(decoded_shares), password);
	}
} // Shamir
