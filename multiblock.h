#ifndef MULTIBLOCK
#define MULTIBLOCK

#include <vector>
#include <cstdint>

class checksummed16: public std::vector<uint8_t> {
	protected:
		size_t data_bits;
	public:
		checksummed16(const std::vector<uint8_t> & data);
		checksummed16(const std::vector<uint8_t> & data, size_t bits);
		std::vector<uint8_t> serialize() const; // returns data with 16bit checksum appended
		bool deserialize(); // checks if last 16 bits is valid checksum and removes it from data
		size_t getLength() const { return data_bits; }
};

struct share {
	public:
		uint16_t index, threshold;
		std::vector<uint8_t> data;
		size_t data_bitlen;
		share(const std::vector<uint8_t> & p);
};

checksummed16 make_share(uint16_t index, uint16_t threshold, const std::vector<uint8_t> & data, size_t data_bitlen);

void f();

#endif
