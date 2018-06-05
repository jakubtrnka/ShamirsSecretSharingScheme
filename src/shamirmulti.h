#ifndef SHAMIRMULTI
#define SHAMIRMULTI

#include <vector>
#include <cstdint>

namespace Shamir {
	std::vector<uint8_t> reconstruct(const std::vector<std::vector<uint8_t>> & share_list);
	std::vector< std::vector<uint8_t> >  distribute(const std::vector<uint8_t> & msg, unsigned threshold, unsigned sharecount);
}

#endif
