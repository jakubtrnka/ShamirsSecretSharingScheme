/**
 * Jakub Trnka, June 2018
 *
 */

#ifndef SLIP39_WRAPPER_H
#define SLIP39_WRAPPER_H 1

#include <vector>
#include <string>

/**
 * fromEnthropy: generates shamir's secret shares from given enthropy
 * randEnthropy: generates shamir's secret shares from randomly generated enthropy of given size
 * toEnthropy: restores enthropy from Shamir's secret shares
 * toSeed: restores enthropy from Shamir's secret shares and performs slip39 specified PBKDF2 to produce seed.
 */
namespace Shamir {
	std::vector<std::vector<std::string>> fromEnthropy(const std::vector<uint8_t> & enthropy, uint16_t count, uint16_t threshold);
	std::vector<std::vector<std::string>> randEnthropy(int bits, uint16_t count, uint16_t threshold, const std::string & password = "");
	std::vector<uint8_t> toEnthropy(const std::vector<std::vector<std::string>> & mnemonics);
	std::vector<uint8_t> toSeed(const std::vector<std::vector<std::string>> & mnemonics, const std::string & password = "");
} // Shamir

#endif
