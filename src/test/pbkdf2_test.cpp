#include <pbkdf2.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace Shamir;

int main() {

	auto s = std::string("tajna zprava");
	auto out = slip39_pbkdf2(std::vector<uint8_t>(s.begin(), s.end()), std::string("heslo"));
	for (auto it: out) std::cout << std::hex << std::setfill('0') << std::setw(2) << (int) it;
	std::cout << '\n';
	return 0;
}
