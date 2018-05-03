#include <wordlist.h>

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <algorithm>

template <size_t SIZE>
void test_mnemonic_creation(const uint8_t raw_enthropy[], const std::array<const std::string, SIZE> & mnemonic) {
	if (SIZE % 3 != 0) throw "number of words not multiple of 3";
	std::vector<uint8_t> enthropy(SIZE*4/3);
	for (auto i=0u; i < SIZE * 4/3; ++i) enthropy[i] = raw_enthropy[i];

	/// Create bip words from initial enthropy and compare with reference
	auto bipwords_num = Shamir::hexToPower2(Shamir::append_bip39_checksum(enthropy), 11);
	for (auto i = 0u; i < mnemonic.size(); ++i) assert( bip_words[bipwords_num[i]] == mnemonic[i]);
	std::cout << "testing the " << SIZE/3*32 << " bit enthropy to match corresponding mnemonic words: passed" << std::endl;
}

template <size_t SIZE>
void test_seed_recovery(const uint8_t raw_enthropy[], const std::array<const std::string, SIZE> & mnemonic) {
	if (SIZE % 3 != 0) throw "number of words not multiple of 3";
	std::vector<uint8_t> enthropy(SIZE*4/3);
	for (auto i=0u; i < SIZE * 4/3; ++i) enthropy[i] = raw_enthropy[i];

	/// restore enthropy from the bip words
	auto bipwords_num = Shamir::bip39ToNum( std::vector<std::string>(mnemonic.begin(), mnemonic.end()));
	auto restored_enthropy = Shamir::power2ToHex(bipwords_num, 11);
	Shamir::check_bip39_checksum(restored_enthropy);
	std::cout << "parsing " << SIZE << "  mnemonic words: correct... ";

	/// compare restored enthropy with original;
	for (auto i=0u; i < restored_enthropy.size(); ++i) assert(raw_enthropy[i] == restored_enthropy[i]);
	std::cout << "testing the " << SIZE << " words mnemonic to match corresponding enthropy: passed" << std::endl;
}

/// data from https://iancoleman.io/bip39/
void bip39_tests() {
	/// more tests for 24-word seeds
	const uint8_t raw_enthropy24_1[] = {0xdd, 0xb4, 0xc4, 0x18, 0x6b, 0xb5, 0x5a, 0xc3, 0xa5, 0x82, 0xc8, 0xa9, 0x47, 0x22, 0xf8, 0xa4, 0x04, 0x0b, 0x1a, 0x9d, 0xa3, 0x57, 0x70, 0x01, 0x45, 0xb9, 0xa9, 0x77, 0x0d, 0xeb, 0x77, 0x3e};
	const std::array<const std::string, 24> mnemonic24_1 = {"tank", "plate", "alcohol", "student", "fiber", "gift", "noodle", "flip", "power", "decorate", "sail", "elite", "doll", "minute", "isolate", "helmet", "hybrid", "any", "hover", "fancy", "ignore", "volcano", "table", "scatter"};
	const uint8_t raw_enthropy24_2[] = {0xa3, 0xba, 0x1a, 0x73, 0xf7, 0x74, 0x7b, 0xdf, 0x30, 0x5a, 0xdc, 0xe8, 0xd5, 0x76, 0xe2, 0xe2, 0xd8, 0x10, 0x3d, 0x36, 0xd7, 0x12, 0x47, 0xb4, 0xa7, 0x4f, 0x0b, 0xf9, 0xea, 0xe3, 0x4b, 0xcc};
	const std::array<const std::string, 24> mnemonic24_2 = {"photo", "speak", "ostrich", "upon", "element", "usage", "scrap", "fortune", "trip", "produce", "reward", "shield", "library", "burden", "swap", "time", "element", "pioneer", "stable", "blood", "pact", "immune", "connect", "office"};
	const uint8_t raw_enthropy24_3[] = {0x90, 0x7a, 0x7b, 0xcb, 0xb0, 0x86, 0x76, 0xc8, 0x94, 0x46, 0x99, 0x89, 0x5f, 0xda, 0xbc, 0xe9, 0xd0, 0xcd, 0x1c, 0x1e, 0x0f, 0xf5, 0x57, 0x4d, 0xe6, 0xc5, 0x69, 0xe9, 0x42, 0xc0, 0x21, 0x73};
	const std::array<const std::string, 24> mnemonic24_3 = {"motor", "stable", "very", "genre", "guess", "gold", "extra", "spring", "matter", "yard", "quality", "squirrel", "art", "mix", "join", "wrong", "firm", "hundred", "rally", "stable", "neither", "gas", "magic", "genuine"};

	const uint8_t raw_enthropy21[] = {0x16, 0xb0, 0xa9, 0x7b, 0xc4, 0xa9, 0x81, 0x58, 0x34, 0x1d, 0x8b, 0x40, 0xa8, 0x9a, 0x90, 0x2d, 0x17, 0xf5, 0xc2, 0xcd, 0x7d, 0xbd, 0x81, 0x80, 0xee, 0x16, 0x6b, 0x31}; 
	const std::array<const std::string, 21> mnemonic21 = {"birth", "luxury", "galaxy", "matter", "object", "project", "spare", "ramp", "doll", "dynamic", "piano", "focus", "leg", "thumb", "curious", "response", "light", "alter", "machine", "stool", "banner"};

	const uint8_t raw_enthropy18[] = {0x91, 0x10, 0xa1, 0x6f, 0xc2, 0x7e, 0xed, 0x25, 0x51, 0xcc, 0x00, 0x54, 0x22, 0x18, 0xc3, 0x79, 0xb9, 0x69, 0x5e, 0x61, 0xa8, 0x3a, 0x55, 0xd9};
	const std::array<const std::string, 18> mnemonic18 = {"much", "lunar", "fossil", "lumber", "uphold", "naive", "elbow", "length", "feature", "awkward", "cotton", "vibrant", "notable", "quality", "mammal", "also", "fiction", "soccer"};

	const uint8_t raw_enthropy15[] = {0x9c, 0x3d, 0x3a, 0xdb, 0x5e, 0xba, 0x58, 0xfb, 0xde, 0x1d, 0x8b, 0x4c, 0x49, 0xa3, 0x27, 0x0d, 0x60, 0x9d, 0x63, 0x1d}; 
	const std::array<const std::string, 15> mnemonic15 = {"order", "truth", "replace", "runway", "pitch", "law", "journey", "ramp", "era", "escape", "six", "bracket", "antenna", "ranch", "insane"};

	/// more tests for 12-word seeds
	const uint8_t raw_enthropy12_1[] = {0x11, 0x1e, 0x4c, 0xe2, 0xfb, 0xb4, 0x8c, 0xc0, 0x3e, 0xe2, 0xf4, 0x3f, 0x5f, 0x54, 0x96, 0xa2};
	const std::array<const std::string, 12> mnemonic12_1 = {"baby", "venue", "december", "wash", "emotion", "gas", "wine", "future", "dismiss", "wide", "certain", "east"};
	const uint8_t raw_enthropy12_2[] = {0x56, 0x38, 0x32, 0x86, 0x31, 0xb7, 0x6e, 0xc6, 0x6a, 0x29, 0x49, 0x69, 0x61, 0x37, 0xc7, 0xb6};
	const std::array<const std::string, 12> mnemonic12_2 = {"file", "screen", "pave", "gloom", "ivory", "glimpse", "post", "circle", "harvest", "answer", "vehicle", "holiday"};
	const uint8_t raw_enthropy12_3[] = {0x45, 0x63, 0xe2, 0x57, 0x5b, 0x3b, 0x1f, 0xb3, 0xe2, 0xf7, 0x2d, 0x12, 0xae, 0x40, 0xd0, 0x98};
	const std::array<const std::string, 12> mnemonic12_3 = {"earth", "business", "noise", "reopen", "rapid", "supreme", "mesh", "toss", "bargain", "improve", "crouch", "crack"};
	
	const uint8_t raw_enthropy9[] = {0xc7, 0xcd, 0x61, 0x94, 0x2a, 0x3f, 0xfc, 0x13, 0xfa, 0x85, 0xff, 0xfa};
	const std::array<const std::string, 9> mnemonic9 = {"sick", "help", "govern", "federal", "zone", "antique", "tube", "say", "wheel"};

	const uint8_t raw_enthropy6[] = {0x3f, 0x79, 0x32, 0x72, 0x74, 0x34, 0x02, 0x5f};
	const std::array<const std::string, 6> mnemonic6 = {"disorder", "sister", "orient", "trick", "dizzy", "copper"};

	const uint8_t raw_enthropy3[] = {0x07, 0x24, 0x6f, 0xea};
	const std::array<const std::string, 3> mnemonic3 = {"already", "casino", "whisper"};

	/// more tests
	test_mnemonic_creation(raw_enthropy24_1, mnemonic24_1);
	test_mnemonic_creation(raw_enthropy24_2, mnemonic24_2);
	test_mnemonic_creation(raw_enthropy24_3, mnemonic24_3);

	test_mnemonic_creation(raw_enthropy21, mnemonic21);
	test_mnemonic_creation(raw_enthropy18, mnemonic18);
	test_mnemonic_creation(raw_enthropy15, mnemonic15);
	/// more tests
	test_mnemonic_creation(raw_enthropy12_1, mnemonic12_1);
	test_mnemonic_creation(raw_enthropy12_2, mnemonic12_2);
	test_mnemonic_creation(raw_enthropy12_3, mnemonic12_3);

	test_mnemonic_creation(raw_enthropy9, mnemonic9);
	test_mnemonic_creation(raw_enthropy6, mnemonic6);
	test_mnemonic_creation(raw_enthropy3, mnemonic3);
	/// more tests
	test_seed_recovery(raw_enthropy24_1, mnemonic24_1);
	test_seed_recovery(raw_enthropy24_2, mnemonic24_2);
	test_seed_recovery(raw_enthropy24_3, mnemonic24_3);

	test_seed_recovery(raw_enthropy21, mnemonic21);
	test_seed_recovery(raw_enthropy18, mnemonic18);
	test_seed_recovery(raw_enthropy15, mnemonic15);
	///more tests
	test_seed_recovery(raw_enthropy12_1, mnemonic12_1);
	test_seed_recovery(raw_enthropy12_2, mnemonic12_2);
	test_seed_recovery(raw_enthropy12_3, mnemonic12_3);

	test_seed_recovery(raw_enthropy9, mnemonic9);
	test_seed_recovery(raw_enthropy6, mnemonic6);
	test_seed_recovery(raw_enthropy3, mnemonic3);
}

void bip39_fail_tests() {
	const uint8_t raw_enthropy24[] = {0xdd, 0xb4, 0xc4, 0x18, 0x6b, 0xb5, 0x5a, 0xc3, 0xa5, 0x82, 0xc8, 0xa9, 0x47, 0x22, 0xf8, 0xa4, 0x04, 0x0b, 0x1a, 0x9d, 0xa3, 0x57, 0x70, 0x01, 0x45, 0xb9, 0xa9, 0x77, 0x0d, 0xeb, 0x77, 0x3f};
	const std::array<const std::string, 24> mnemonic24 = {"tank", "plate", "alcohol", "student", "fiber", "gift", "noodle", "flip", "power", "decorate", "sail", "elite", "doll", "minute", "isolate", "helmet", "hybrid", "any", "hover", "fancy", "ignore", "volcano", "table", "scatter"};
	try {
		test_mnemonic_creation(raw_enthropy24, mnemonic24);
	} catch ( const char * s) {
		std::cout << s << std::endl;
	}
}

int main() {
	bip39_tests();
	//bip39_fail_tests();
	return 0;
}
