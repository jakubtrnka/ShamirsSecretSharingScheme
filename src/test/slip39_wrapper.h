#ifndef SLIP39_WRAPPER_H
#define SLIP39_WRAPPER_H 1

#include <vector>
#include <string>

namespace Shamir {
	std::vector<std::vector<std::string>> fromEnthropy(const std::vector<uint8_t> & enthropy, uint16_t count, uint16_t threshold, const std::string & password = "");
	std::vector<std::vector<std::string>> randEnthropy(int bits, uint16_t count, uint16_t threshold, const std::string & password = "");
	std::vector<uint8_t> toEnthropy(const std::vector<std::vector<std::string>> & mnemonics, const std::string & password = "");
} // Shamir

#endif
