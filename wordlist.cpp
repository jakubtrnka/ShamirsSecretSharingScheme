#include <wordlist.h>
#include <sha256.h>

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

namespace {
	std::vector<uint8_t> append_bip39_checksum(const std::vector<uint8_t> & it) {
		std::vector<uint8_t> output(it);
		CSHA256 h;
		uint8_t hash[32];
		h.Write(it.data(), it.size());
		h.Finalize(hash);
		int seedchunks = it.size() / 4;
		for (auto i=0; i< seedchunks/8; ++i) output.push_back(hash[i]);
		output.push_back(hash[seedchunks/8] & (0xff << (seedchunks % 8)));
		return output;
	}
}

std::vector<int> hexToPower2(const std::vector<uint8_t> & data, int p) {
	if ( p < 9 || p > 24 ) throw "base 2-power must be between 9 and 24";
	std::vector<int> output;
	uint32_t bitholder(0);
	int bitsread(0);
	for (auto & x: data) {
		int willread = (((p - bitsread) > 8) ? 8 : (p - bitsread));
		bitholder <<= willread;
		bitholder |= (x >> (8 - willread));
		bitsread += willread;
		if (bitsread == p) {
			output.push_back(bitholder);
			bitholder = x & (0xff >> (willread));
			bitsread = 8 - willread;
		}
	}
	int last = bitholder << (p - bitsread);
	if (last != 0) output.push_back(last);
	return output;
}

std::vector<int> hexTo2048(const std::vector<uint8_t> & data) {
	std::vector<int> output;
	uint32_t bitholder(0);
	int bitsread(0);
	for (auto & x: data) {
		int willread = (((11 - bitsread) > 8) ? 8 : (11 - bitsread));
		bitholder <<= willread;
		bitholder |= (x >> (8 - willread));
		bitsread += willread;
		if (bitsread == 11) {
			output.push_back(bitholder);
			bitholder = x & (0xff >> (willread));
			bitsread = 8 - willread;
		}
	}
	int last = bitholder << (11 - bitsread);
	if (last != 0) output.push_back(last);
	return output;
}

std::vector<int> hexTo1024(const std::vector<uint8_t> & data) {
	std::vector<int> output;
	uint32_t bitholder(0);
	int bitsread(0);
	for (auto & x: data) {
		int willread = (((10 - bitsread) > 8) ? 8 : (10 - bitsread));
		bitholder <<= willread;
		bitholder |= (x >> (8 - willread));
		bitsread += willread;
		if (bitsread == 10) {
			output.push_back(bitholder);
			bitholder = x & (0xff >> (willread));
			bitsread = 8 - willread;
		}
	}
	int last = bitholder << (10 - bitsread);
	if (last != 0) output.push_back(last);
	return output;
}

void f() {
	const uint8_t testvec[] = {0x19, 0xd7, 0x11, 0xec, 0x58, 0x0f, 0x43, 0x82, 0x9a, 0xfb, 0x45, 0x1b, 0x28, 0x71, 0xd3, 0x6e};
	std::vector<uint8_t> testdata(std::begin(testvec), std::end(testvec));
	auto bip_numbers = hexToPower2(append_bip39_checksum(testdata), 11);
	auto slip_numbers = hexToPower2(testdata, 10);

	std::cout << bip_words[bip_numbers[0]];
	for (unsigned i=1; i<bip_numbers.size(); ++i) std::cout << " " << bip_words[bip_numbers[i]];
	std::cout << std::endl;
	std::cout << "border review kitchen quote vintage scout hip hamster brass drum inquiry tag <- ref\n" << std::endl;

	std::cout << slip_words[slip_numbers[0]];
	for (unsigned i=1; i<slip_numbers.size(); ++i) std::cout << " " << slip_words[slip_numbers[i]];
	std::cout << std::endl << std::hex;

}


/*
 *
 * 19d711ec580f43829afb451b2871d36e == "border review kitchen quote vintage scout hip hamster brass drum inquiry tag"
 */
