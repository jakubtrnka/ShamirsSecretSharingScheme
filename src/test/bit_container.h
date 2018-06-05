#ifndef BIT_CONTAINER_H
#define BIT_CONTAINER_H 1

#include <cstdint>
#include <vector>

namespace {
	typedef std::vector<uint8_t> raw_data;
}

class bit_container: public raw_data {
	public:
		bit_container(const raw_data & v) : raw_data(v) { height = v.size() * 8; }
		bit_container() : height(0) {}
		raw_data pop_n(size_t n);
		void push_n(const raw_data & data, size_t n);
		size_t get_height() const { return height; }
	private:
		size_t height;
};

#endif
