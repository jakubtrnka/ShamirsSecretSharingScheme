#ifndef MULTIBLOCK
#define MULTIBLOCK

#include <vector>
#include <cstdint>

/**
 * holds data with length 'data_bits' bits in a vector,
 * produces 16bits SHA256 checksum upon serialization,
 * but doesn't store it. Can parse data with checksum,
 * verify checksum and keep only data.
 */
class checksummed16: public std::vector<uint8_t> {
	protected:
		size_t data_bits;
	public:
		checksummed16(const std::vector<uint8_t> & data);
		checksummed16(const std::vector<uint8_t> & data, size_t bits);
		std::vector<uint8_t> serialize() const; // returns data with 16bit checksum appended
		bool deserialize(); // checks if last 16 bits is valid checksum and removes it from data if so.
		size_t getLength() const { return data_bits; }
};

/**
 * data structure that holds Shamir's Secret share according to SLIP0039:
 * index, threshold: 5bit numbers starting from 1
 * data: vector of 'n*8' bits of payload
 */
struct share {
	public:
		uint16_t index, threshold;
		std::vector<uint8_t> data;
		share(const std::vector<uint8_t> & p);
};

namespace Shamir {
	checksummed16 make_share(uint16_t index, uint16_t threshold, const std::vector<uint8_t> & data);
}

#endif
