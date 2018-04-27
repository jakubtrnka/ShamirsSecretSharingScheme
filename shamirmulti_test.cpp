#include <shamirmulti.h>

#include <iostream>
#include <iomanip>
#include <vector>

namespace {
	std::ostream & myout (std::ostream & ost) { return ost << std::hex << std::setfill('0') << std::setw(2); }
}

int main() {
	uint8_t asdf(64);
	std::cout << myout << (int) asdf;
	return 0;
}
