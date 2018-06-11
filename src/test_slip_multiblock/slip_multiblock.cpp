#include <share.h>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>

using namespace Shamir;

void reconstruct() {
	std::vector<std::vector<uint8_t>> shares;
	std::string s;
	while (std::cin) {
		shares.push_back(std::vector<uint8_t>());
		unsigned tmp;
		std::cin >> s;
		if (std::cin)
			for(unsigned i=0; i<s.size()/2; ++i)
				if (sscanf(s.c_str() + i*2, "%02x", &tmp) == 1) shares.back().push_back((uint8_t) tmp);
		if (shares.back().empty()) shares.pop_back();
	}

	auto out = reconstruct_secret_slip(shares);
	//for (auto it: out) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int) it;
	for (auto it: out) std::cout << it;
	std::cout << '\n';
}

void decompose() {
	std::string sec("Toto je velice tajna zprava.");
	std::vector<uint8_t> sec_v(sec.begin(), sec.end());
	try {
		std::vector<bit_container> out = distribute_secret_slip(sec_v, 10u, 3u);
		for (auto &it: out) {
			//append_checksum(out);
			for (auto jt: it) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int) jt;
			std::cout << '\n';
		}
	} catch (const char * s) {
		std::cerr << s << '\n';
	}
}

int main() {
#if 1
	try {
		reconstruct();
	} catch (const char *s) {
		std::cerr << s << std::endl;
	}
#else
	decompose();
#endif
	return 0;
}
