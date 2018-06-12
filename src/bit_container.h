#ifndef BIT_CONTAINER_H
#define BIT_CONTAINER_H 1

#include <cstdint>
#include <vector>

namespace Shamir {
	class bit_container: public std::vector<uint8_t> {
		public:
			bit_container(const std::vector<uint8_t> & v) : std::vector<uint8_t>(v) { height = v.size() * 8; }
			bit_container() : height(0) {}
			void pop_n(std::vector<uint8_t> & output, size_t n);
			void pop_n(size_t n);
			void push_n(const std::vector<uint8_t> & data, size_t n);
			size_t get_height() const { return height; }
		private:
			size_t height;
	};
} // Shamir

#endif
