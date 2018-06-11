#ifndef MULTIBLOCKSHAMIR_H
#define MULTIBLOCKSHAMIR_H 1

#include <oneblockshamir.h>

#include <vector>
#include <cstdint>

namespace Shamir {
	std::vector<uint8_t> reconstruct_secret_raw(const std::vector<std::vector<xy_point>> & raw_shares);
	std::vector<std::vector<uint8_t>> distribute_secret_raw(const std::vector<uint8_t> & secret, uint16_t count, uint16_t threshold);
} // Shamir
#endif
