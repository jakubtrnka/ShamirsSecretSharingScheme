#include <bit_container.h>
#include <endian.h>
#include <stdexcept>

#include <iostream>

namespace {
	union overflower {
		uint8_t array[2];
		uint16_t number;
	};

}

raw_data bit_container::pop_n(size_t n) {
	if (n > height) throw std::underflow_error("requested more bits that there are in the container\n");
	raw_data output;
	const size_t start = (height-n)/8;
	const size_t stop = height / 8 - ((height % 8 == 0)? 1 : 0 );
	const short shift = (short) ((height - n) % 8);
	//std::cout << "(((" << start << ":" << shift << "--" << stop << ";" << height % 8 << ")))\n";
	overflower carry{0};
	for (auto iter = start; iter <= stop; ++iter) {
		carry.number = this->at(iter);
		carry.number <<= shift;
		carry.number = htobe16(carry.number);
		//std::cout << "[[[" << (int)carry.array[1] << "]]]";
		if (!output.empty()) output.back() |= carry.array[0];
		output.push_back(carry.array[1]);
	}
	height -= n;
	this->resize(height/8 + ((height % 8 == 0) ? 0 : 1 ));
	if (! (this->empty())) this->back() &= (0xff << ((8 - (height % 8)) % 8));
	return output;
}

void bit_container::push_n(const raw_data & data, size_t n) {
	size_t appended(0);
	short overflow = (short) (height % 8);
	for (auto it: data) {
		overflower carry;
		carry.number = it;
		if (n - appended >= 8) {
			appended += 8;
		} else {

			carry.number &= (0xffff << (8 + appended - n));
			appended = n;
		}
		carry.number <<= 8 - overflow;
		if (overflow == 0) carry.number >>= 8;
		carry.number = htobe16(carry.number);
		if (overflow != 0) this->back() |= carry.array[0];
		this->push_back(carry.array[1]);

	}
	height += appended;
	this->resize(height/8 + ((height % 8) ? 1 : 0));
}
