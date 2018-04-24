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
		bool append(std::vector<uint8_t> & data);
		bool append(std::vector<uint8_t> & data, size_t bits);
		std::vector<uint8_t> serialize() const; // returns data with 16bit checksum appended
		bool deserialize(); // checks if last 16 bits is valid checksum and removes it from data
		size_t getLength() const { return data_bits; }
};



void f();

#endif
