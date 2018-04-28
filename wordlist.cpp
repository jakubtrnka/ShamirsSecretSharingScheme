#include <wordlist.h>

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>


std::vector<std::string> hexTo2048(const std::vector<uint8_t> & data) {
	std::vector<std::string> output;
	uint32_t bitholder(0);
	int bitsread(0);
	for (auto & x: data) {
		int willread = (((11 - bitsread) > 8) ? 8 : (11 - bitsread));
		bitholder <<= willread;
		bitholder |= (x >> (8 - willread));
		bitsread += willread;
		if (bitsread == 11) {
			output.push_back(bip_words[bitholder]);
			bitholder = x & (0xff >> (willread));
			bitsread = 8 - willread;
		}
	}
	output.push_back(bip_words[(bitholder << (11 - bitsread))]);
	return output;
}

std::vector<std::string> hexTo1024(const std::vector<uint8_t> & data) {
	std::vector<std::string> output;
	uint32_t bitholder(0);
	int bitsread(0);
	for (auto & x: data) {
		int willread = (((10 - bitsread) > 8) ? 8 : (10 - bitsread));
		bitholder <<= willread;
		bitholder |= (x >> (8 - willread));
		bitsread += willread;
		if (bitsread == 10) {
			output.push_back(slip_words[bitholder]);
			bitholder = x & (0xff >> (willread));
			bitsread = 8 - willread;
		}
	}
	output.push_back(slip_words[(bitholder << (10 - bitsread))]);
	return output;
}

void f() {
	const uint8_t testvec[] = {0x19, 0xd7, 0x11, 0xec, 0x58, 0x0f, 0x43, 0x82, 0x9a, 0xfb, 0x45, 0x1b, 0x28, 0x71, 0xd3, 0x6e};
	std::vector<uint8_t> testdata(std::begin(testvec), std::end(testvec));
	auto bip_words = hexTo2048(testdata);
	auto slip_words = hexTo1024(testdata);

	std::cout << bip_words[0];
	for (unsigned i=1; i<bip_words.size(); ++i) std::cout << " " << bip_words[i];
	std::cout << std::endl;
	std::cout << "border review kitchen quote vintage scout hip hamster brass drum inquiry tag <- ref\n" << std::endl;

	std::cout << slip_words[0];
	for (unsigned i=1; i<slip_words.size(); ++i) std::cout << " " << slip_words[i];
	std::cout << std::endl;

}


/*
 *
 * 19d711ec580f43829afb451b2871d36e == "border review kitchen quote vintage scout hip hamster brass drum inquiry tag"
 */
