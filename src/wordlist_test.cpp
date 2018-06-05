#include <wordlist.h>

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <algorithm>

template <size_t SIZE1, size_t SIZE2>
void test_mnemonic_creation(const std::array<uint8_t, SIZE1> & raw_enthropy, const std::array<const std::string, SIZE2> & mnemonic) {
	if (SIZE2 % 3 != 0) throw "number of words not multiple of 3";
	std::vector<uint8_t> enthropy(raw_enthropy.begin(), raw_enthropy.end());

	/// Create bip words from initial enthropy and compare with reference
	auto bipwords_num = Shamir::hexToPower2(Shamir::append_bip39_checksum(enthropy), 11);
	for (auto i = 0u; i < mnemonic.size(); ++i) assert( bip_words[bipwords_num[i]] == mnemonic[i]);
	//std::cout << "testing the " << SIZE/3*32 << " bit enthropy to match corresponding mnemonic words: passed" << std::endl;
	std::cout << "testing the " << SIZE1 << " bit enthropy to match corresponding mnemonic words: passed" << std::endl;
}

template <size_t SIZE1, size_t SIZE2>
void test_seed_recovery(const std::array<uint8_t, SIZE1> & raw_enthropy, const std::array<const std::string, SIZE2> & mnemonic) {
	if (SIZE2 % 3 != 0) throw "number of words not multiple of 3";
	std::vector<uint8_t> enthropy(raw_enthropy.begin(), raw_enthropy.end());

	/// restore enthropy from the bip words
	auto bipwords_num = Shamir::bip39ToNum( std::vector<std::string>(mnemonic.begin(), mnemonic.end()));
	auto restored_enthropy = Shamir::power2ToHex(bipwords_num, 11);
	Shamir::check_bip39_checksum(restored_enthropy);
	std::cout << "parsing " << SIZE2 << "  mnemonic words: bip39 checksum ok... ";

	/// compare restored enthropy with original;
	for (auto i=0u; i < restored_enthropy.size(); ++i) assert(raw_enthropy[i] == restored_enthropy[i]);
	std::cout << "testing the " << SIZE2 << " words mnemonic to match corresponding enthropy: passed" << std::endl;
}

/// data from https://iancoleman.io/bip39/
void bip39_tests() {
	/// more tests for 24-word seeds and 12-word seeds
	const std::array<uint8_t, 32> raw_enthropy24_1 = {0xdd, 0xb4, 0xc4, 0x18, 0x6b, 0xb5, 0x5a, 0xc3, 0xa5, 0x82, 0xc8, 0xa9, 0x47, 0x22, 0xf8, 0xa4, 0x04, 0x0b, 0x1a, 0x9d, 0xa3, 0x57, 0x70, 0x01, 0x45, 0xb9, 0xa9, 0x77, 0x0d, 0xeb, 0x77, 0x3e};
	const std::array<uint8_t, 32> raw_enthropy24_2 = {0xa3, 0xba, 0x1a, 0x73, 0xf7, 0x74, 0x7b, 0xdf, 0x30, 0x5a, 0xdc, 0xe8, 0xd5, 0x76, 0xe2, 0xe2, 0xd8, 0x10, 0x3d, 0x36, 0xd7, 0x12, 0x47, 0xb4, 0xa7, 0x4f, 0x0b, 0xf9, 0xea, 0xe3, 0x4b, 0xcc};
	const std::array<uint8_t, 32> raw_enthropy24_3 = {0x90, 0x7a, 0x7b, 0xcb, 0xb0, 0x86, 0x76, 0xc8, 0x94, 0x46, 0x99, 0x89, 0x5f, 0xda, 0xbc, 0xe9, 0xd0, 0xcd, 0x1c, 0x1e, 0x0f, 0xf5, 0x57, 0x4d, 0xe6, 0xc5, 0x69, 0xe9, 0x42, 0xc0, 0x21, 0x73};
	const std::array<uint8_t, 28> raw_enthropy21 = {0x16, 0xb0, 0xa9, 0x7b, 0xc4, 0xa9, 0x81, 0x58, 0x34, 0x1d, 0x8b, 0x40, 0xa8, 0x9a, 0x90, 0x2d, 0x17, 0xf5, 0xc2, 0xcd, 0x7d, 0xbd, 0x81, 0x80, 0xee, 0x16, 0x6b, 0x31}; 
	const std::array<uint8_t, 24> raw_enthropy18 = {0x91, 0x10, 0xa1, 0x6f, 0xc2, 0x7e, 0xed, 0x25, 0x51, 0xcc, 0x00, 0x54, 0x22, 0x18, 0xc3, 0x79, 0xb9, 0x69, 0x5e, 0x61, 0xa8, 0x3a, 0x55, 0xd9};
	const std::array<uint8_t, 20> raw_enthropy15 = {0x9c, 0x3d, 0x3a, 0xdb, 0x5e, 0xba, 0x58, 0xfb, 0xde, 0x1d, 0x8b, 0x4c, 0x49, 0xa3, 0x27, 0x0d, 0x60, 0x9d, 0x63, 0x1d}; 
	const std::array<uint8_t, 16> raw_enthropy12_1 = {0x11, 0x1e, 0x4c, 0xe2, 0xfb, 0xb4, 0x8c, 0xc0, 0x3e, 0xe2, 0xf4, 0x3f, 0x5f, 0x54, 0x96, 0xa2};
	const std::array<uint8_t, 16> raw_enthropy12_2 = {0x56, 0x38, 0x32, 0x86, 0x31, 0xb7, 0x6e, 0xc6, 0x6a, 0x29, 0x49, 0x69, 0x61, 0x37, 0xc7, 0xb6};
	const std::array<uint8_t, 16> raw_enthropy12_3 = {0x45, 0x63, 0xe2, 0x57, 0x5b, 0x3b, 0x1f, 0xb3, 0xe2, 0xf7, 0x2d, 0x12, 0xae, 0x40, 0xd0, 0x98};
	const std::array<uint8_t, 12> raw_enthropy9 = {0xc7, 0xcd, 0x61, 0x94, 0x2a, 0x3f, 0xfc, 0x13, 0xfa, 0x85, 0xff, 0xfa};
	const std::array<uint8_t, 8> raw_enthropy6 = {0x3f, 0x79, 0x32, 0x72, 0x74, 0x34, 0x02, 0x5f};
	const std::array<uint8_t, 4> raw_enthropy3 = {0x07, 0x24, 0x6f, 0xea};
	const std::array<const std::string, 24> mnemonic24_1 = {"tank", "plate", "alcohol", "student", "fiber", "gift", "noodle", "flip", "power", "decorate", "sail", "elite", "doll", "minute", "isolate", "helmet", "hybrid", "any", "hover", "fancy", "ignore", "volcano", "table", "scatter"};
	const std::array<const std::string, 24> mnemonic24_2 = {"photo", "speak", "ostrich", "upon", "element", "usage", "scrap", "fortune", "trip", "produce", "reward", "shield", "library", "burden", "swap", "time", "element", "pioneer", "stable", "blood", "pact", "immune", "connect", "office"};
	const std::array<const std::string, 24> mnemonic24_3 = {"motor", "stable", "very", "genre", "guess", "gold", "extra", "spring", "matter", "yard", "quality", "squirrel", "art", "mix", "join", "wrong", "firm", "hundred", "rally", "stable", "neither", "gas", "magic", "genuine"};
	const std::array<const std::string, 21> mnemonic21 = {"birth", "luxury", "galaxy", "matter", "object", "project", "spare", "ramp", "doll", "dynamic", "piano", "focus", "leg", "thumb", "curious", "response", "light", "alter", "machine", "stool", "banner"};
	const std::array<const std::string, 18> mnemonic18 = {"much", "lunar", "fossil", "lumber", "uphold", "naive", "elbow", "length", "feature", "awkward", "cotton", "vibrant", "notable", "quality", "mammal", "also", "fiction", "soccer"};
	const std::array<const std::string, 15> mnemonic15 = {"order", "truth", "replace", "runway", "pitch", "law", "journey", "ramp", "era", "escape", "six", "bracket", "antenna", "ranch", "insane"};
	const std::array<const std::string, 12> mnemonic12_1 = {"baby", "venue", "december", "wash", "emotion", "gas", "wine", "future", "dismiss", "wide", "certain", "east"};
	const std::array<const std::string, 12> mnemonic12_2 = {"file", "screen", "pave", "gloom", "ivory", "glimpse", "post", "circle", "harvest", "answer", "vehicle", "holiday"};
	const std::array<const std::string, 12> mnemonic12_3 = {"earth", "business", "noise", "reopen", "rapid", "supreme", "mesh", "toss", "bargain", "improve", "crouch", "crack"};
	const std::array<const std::string, 9> mnemonic9 = {"sick", "help", "govern", "federal", "zone", "antique", "tube", "say", "wheel"};
	const std::array<const std::string, 6> mnemonic6 = {"disorder", "sister", "orient", "trick", "dizzy", "copper"};
	const std::array<const std::string, 3> mnemonic3 = {"already", "casino", "whisper"};

	std::cout << "BIP0039:\n";
	/// more tests for 24 and 12 word mnemonics
	test_mnemonic_creation(raw_enthropy24_1, mnemonic24_1);
	test_mnemonic_creation(raw_enthropy24_2, mnemonic24_2);
	test_mnemonic_creation(raw_enthropy24_3, mnemonic24_3);
	test_mnemonic_creation(raw_enthropy21, mnemonic21);
	test_mnemonic_creation(raw_enthropy18, mnemonic18);
	test_mnemonic_creation(raw_enthropy15, mnemonic15);
	test_mnemonic_creation(raw_enthropy12_1, mnemonic12_1);
	test_mnemonic_creation(raw_enthropy12_2, mnemonic12_2);
	test_mnemonic_creation(raw_enthropy12_3, mnemonic12_3);
	test_mnemonic_creation(raw_enthropy9, mnemonic9);
	test_mnemonic_creation(raw_enthropy6, mnemonic6);
	test_mnemonic_creation(raw_enthropy3, mnemonic3);
	std::cout << "---------------------------------" << std::endl;
	test_seed_recovery(raw_enthropy24_1, mnemonic24_1);
	test_seed_recovery(raw_enthropy24_2, mnemonic24_2);
	test_seed_recovery(raw_enthropy24_3, mnemonic24_3);
	test_seed_recovery(raw_enthropy21, mnemonic21);
	test_seed_recovery(raw_enthropy18, mnemonic18);
	test_seed_recovery(raw_enthropy15, mnemonic15);
	test_seed_recovery(raw_enthropy12_1, mnemonic12_1);
	test_seed_recovery(raw_enthropy12_2, mnemonic12_2);
	test_seed_recovery(raw_enthropy12_3, mnemonic12_3);
	test_seed_recovery(raw_enthropy9, mnemonic9);
	test_seed_recovery(raw_enthropy6, mnemonic6);
	test_seed_recovery(raw_enthropy3, mnemonic3);
	std::cout << "---------------------------------\nAll BIP39 encoding/decoding tests passed successfully\n" << std::endl;
}

template <size_t SIZE, size_t SIZE2>
void slip39_decode_test(const std::array<uint8_t, SIZE> & v, const std::array<int, SIZE2> & ref) {
	std::vector<int> nums(ref.begin(), ref.end());
	auto represHex = Shamir::power2ToHex(nums, 10);
	for (auto i = 0u; i < v.size(); ++i) assert(v[i] == represHex[i]);
	if (v.size() + 1 == represHex.size()) /// last SLIP39 word is zero-padded, which might be appended as zero-byte if it doesn't fit into whole octet.
		assert(represHex.back() == 0);
	std::cout << "Hexadecimal representation of " << SIZE*8 << "-bit data restored successfully\n";
}

template <size_t SIZE, size_t SIZE2>
void slip39_encode_test(const std::array<uint8_t, SIZE> & v, const std::array<int, SIZE2> & ref) {
	std::vector<uint8_t> bytes(v.begin(), v.end());
	auto repres10 = Shamir::hexToPower2(bytes, 10);
	assert( repres10.size() == ref.size() );
	for (auto i=0u; i < repres10.size(); ++i) assert(ref[i] == repres10[i]);
	std::cout << "10-bit representation of " << SIZE*8 << "-bit data verified successfully\n";
}

template<size_t SIZE>
void slip39_parse_words_test(const std::array<std::string, SIZE> & words, const std::array<int, SIZE> & nums) {
	std::vector<std::string> tmp(words.begin(), words.end());
	auto restored = Shamir::slip39ToNum(tmp);
	assert(restored.size() == nums.size());
	for (auto i = 0u; i < restored.size(); ++i) assert(restored[i] == nums[i]);
	std::cout << SIZE << " SLIP39 words parsed successfully\n";
}

void slip39_tests() {
	const std::array<uint8_t,32> raw_enthropy32_1 = {0xdd, 0xb4, 0xc4, 0x18, 0x6b, 0xb5, 0x5a, 0xc3, 0xa5, 0x82, 0xc8, 0xa9, 0x47, 0x22, 0xf8, 0xa4, 0x04, 0x0b, 0x1a, 0x9d, 0xa3, 0x57, 0x70, 0x01, 0x45, 0xb9, 0xa9, 0x77, 0x0d, 0xeb, 0x77, 0x3e};
	const std::array<uint8_t,32> raw_enthropy32_2 = {0xa3, 0xba, 0x1a, 0x73, 0xf7, 0x74, 0x7b, 0xdf, 0x30, 0x5a, 0xdc, 0xe8, 0xd5, 0x76, 0xe2, 0xe2, 0xd8, 0x10, 0x3d, 0x36, 0xd7, 0x12, 0x47, 0xb4, 0xa7, 0x4f, 0x0b, 0xf9, 0xea, 0xe3, 0x4b, 0xcc};
	const std::array<uint8_t,32> raw_enthropy32_3 = {0x90, 0x7a, 0x7b, 0xcb, 0xb0, 0x86, 0x76, 0xc8, 0x94, 0x46, 0x99, 0x89, 0x5f, 0xda, 0xbc, 0xe9, 0xd0, 0xcd, 0x1c, 0x1e, 0x0f, 0xf5, 0x57, 0x4d, 0xe6, 0xc5, 0x69, 0xe9, 0x42, 0xc0, 0x21, 0x73};
	const std::array<uint8_t,28> raw_enthropy28 = {0x16, 0xb0, 0xa9, 0x7b, 0xc4, 0xa9, 0x81, 0x58, 0x34, 0x1d, 0x8b, 0x40, 0xa8, 0x9a, 0x90, 0x2d, 0x17, 0xf5, 0xc2, 0xcd, 0x7d, 0xbd, 0x81, 0x80, 0xee, 0x16, 0x6b, 0x31}; 
	const std::array<uint8_t,24> raw_enthropy24 = {0x91, 0x10, 0xa1, 0x6f, 0xc2, 0x7e, 0xed, 0x25, 0x51, 0xcc, 0x00, 0x54, 0x22, 0x18, 0xc3, 0x79, 0xb9, 0x69, 0x5e, 0x61, 0xa8, 0x3a, 0x55, 0xd9};
	const std::array<uint8_t,20> raw_enthropy20 = {0x9c, 0x3d, 0x3a, 0xdb, 0x5e, 0xba, 0x58, 0xfb, 0xde, 0x1d, 0x8b, 0x4c, 0x49, 0xa3, 0x27, 0x0d, 0x60, 0x9d, 0x63, 0x1d}; 
	const std::array<uint8_t,16> raw_enthropy16_1 = {0x11, 0x1e, 0x4c, 0xe2, 0xfb, 0xb4, 0x8c, 0xc0, 0x3e, 0xe2, 0xf4, 0x3f, 0x5f, 0x54, 0x96, 0xa2};
	const std::array<uint8_t,16> raw_enthropy16_2 = {0x56, 0x38, 0x32, 0x86, 0x31, 0xb7, 0x6e, 0xc6, 0x6a, 0x29, 0x49, 0x69, 0x61, 0x37, 0xc7, 0xb6};
	const std::array<uint8_t,16> raw_enthropy16_3 = {0x45, 0x63, 0xe2, 0x57, 0x5b, 0x3b, 0x1f, 0xb3, 0xe2, 0xf7, 0x2d, 0x12, 0xae, 0x40, 0xd0, 0x98};
	const std::array<uint8_t,12> raw_enthropy12 = {0xc7, 0xcd, 0x61, 0x94, 0x2a, 0x3f, 0xfc, 0x13, 0xfa, 0x85, 0xff, 0xfa};
	const std::array<uint8_t,8> raw_enthropy8 = {0x3f, 0x79, 0x32, 0x72, 0x74, 0x34, 0x02, 0x5f};
	const std::array<uint8_t,4> raw_enthropy4 = {0x07, 0x24, 0x6f, 0xea};
	const std::array<int,26> raw_enthropy32_1_10rep = {886, 844, 262, 107, 725, 428, 233, 386, 802, 660, 456, 760, 656, 64, 710, 669, 653, 375, 0, 325, 742, 663, 451, 491, 476, 992};
	const std::array<int,26> raw_enthropy32_2_10rep = {654, 929, 668, 1015, 465, 957, 972, 90, 883, 653, 349, 738, 907, 385, 15, 310, 860, 292, 493, 167, 316, 191, 634, 739, 303, 192};
	const std::array<int,26> raw_enthropy32_3_10rep = {577, 935, 754, 944, 537, 876, 549, 70, 614, 149, 1014, 700, 935, 268, 839, 30, 63, 853, 467, 486, 789, 670, 592, 704, 133, 816};
	const std::array<int,23> raw_enthropy28_10rep = {90, 778, 606, 964, 678, 21, 525, 29, 557, 10, 550, 656, 180, 383, 368, 717, 502, 984, 96, 238, 89, 691, 64};
	const std::array<int,20> raw_enthropy24_10rep = {580, 266, 91, 962, 507, 722, 340, 460, 1, 322, 134, 195, 486, 918, 599, 609, 672, 933, 374, 256};
	const std::array<int,16> raw_enthropy20_10rep = {624, 979, 694, 862, 745, 399, 759, 541, 557, 196, 616, 807, 53, 521, 856, 797};
	const std::array<int,13> raw_enthropy16_1_10rep = {68, 484, 824, 763, 722, 204, 15, 738, 976, 1013, 981, 150, 648};
	const std::array<int,13> raw_enthropy16_2_10rep = {344, 899, 161, 561, 733, 748, 410, 553, 293, 662, 77, 967, 728};
	const std::array<int,13> raw_enthropy16_3_10rep = {277, 574, 149, 859, 236, 507, 248, 759, 180, 298, 912, 208, 608};
	const std::array<int,10> raw_enthropy12_10rep = {799, 214, 101, 42, 255, 961, 254, 645, 1023, 928};
	const std::array<int,7> raw_enthropy8_10rep = {253, 915, 156, 628, 208, 37, 960};
	const std::array<int,4> raw_enthropy4_10rep = {28, 582, 1018, 512};
	const std::array<std::string,26> raw_enthropy32_1_slip_words = {"tenant", "squeeze", "dwarf", "brown", "remove", "heart", "diamond", "fury", "silver", "pipe", "index", "satisfy", "picture", "awkward", "raven", "pluck", "physical", "frame", "academic", "expand", "risk", "planet", "improve", "kitchen", "jacket", "warning"};
	const std::array<std::string,26> raw_enthropy32_2_slip_words = {"piano", "twenty", "pledge", "worth", "insane", "vague", "video", "blossom", "taxi", "physical", "field", "rhythm", "torch", "furnace", "agree", "evidence", "strike", "enforce", "kiwi", "coil", "exchange", "crowd", "parade", "rich", "erode", "crucial"};
	const std::array<std::string,16> raw_enthropy20_slip_words = {"oyster", "vital", "purpose", "style", "road", "glad", "salt", "manage", "melt", "crystal", "orbit", "size", "atom", "limit", "still", "shoulder"};
	const std::array<std::string,13> raw_enthropy16_2_slip_words = {"fatal", "tired", "club", "mesh", "response", "rocket", "great", "maximum", "engage", "pitch", "betray", "very", "repeat"};

	std::cout << "SLIP0039: 10-bit representation\n";
	/// encode
	slip39_encode_test(raw_enthropy32_1, raw_enthropy32_1_10rep);
	slip39_encode_test(raw_enthropy32_2, raw_enthropy32_2_10rep);
	slip39_encode_test(raw_enthropy32_3, raw_enthropy32_3_10rep);
	slip39_encode_test(raw_enthropy28, raw_enthropy28_10rep);
	slip39_encode_test(raw_enthropy24, raw_enthropy24_10rep);
	slip39_encode_test(raw_enthropy20, raw_enthropy20_10rep);
	slip39_encode_test(raw_enthropy16_1, raw_enthropy16_1_10rep);
	slip39_encode_test(raw_enthropy16_2, raw_enthropy16_2_10rep);
	slip39_encode_test(raw_enthropy16_3, raw_enthropy16_3_10rep);
	slip39_encode_test(raw_enthropy12, raw_enthropy12_10rep);
	slip39_encode_test(raw_enthropy8, raw_enthropy8_10rep);
	slip39_encode_test(raw_enthropy4, raw_enthropy4_10rep);
	std::cout << "---------------------------------" << std::endl;
	/// decode
	slip39_decode_test(raw_enthropy32_1, raw_enthropy32_1_10rep);
	slip39_decode_test(raw_enthropy32_2, raw_enthropy32_2_10rep);
	slip39_decode_test(raw_enthropy32_3, raw_enthropy32_3_10rep);
	slip39_decode_test(raw_enthropy28, raw_enthropy28_10rep);
	slip39_decode_test(raw_enthropy24, raw_enthropy24_10rep);
	slip39_decode_test(raw_enthropy20, raw_enthropy20_10rep);
	slip39_decode_test(raw_enthropy16_1, raw_enthropy16_1_10rep);
	slip39_decode_test(raw_enthropy16_2, raw_enthropy16_2_10rep);
	slip39_decode_test(raw_enthropy16_3, raw_enthropy16_3_10rep);
	slip39_decode_test(raw_enthropy12, raw_enthropy12_10rep);
	slip39_decode_test(raw_enthropy8, raw_enthropy8_10rep);
	slip39_decode_test(raw_enthropy4, raw_enthropy4_10rep);
	std::cout << "---------------------------------" << std::endl;
	slip39_parse_words_test(raw_enthropy32_1_slip_words, raw_enthropy32_1_10rep);
	slip39_parse_words_test(raw_enthropy32_2_slip_words, raw_enthropy32_2_10rep);
	slip39_parse_words_test(raw_enthropy20_slip_words, raw_enthropy20_10rep);
	slip39_parse_words_test(raw_enthropy16_2_slip_words, raw_enthropy16_2_10rep);
	std::cout << "---------------------------------\nAll SLIP39 encoding/decoding tests passed successfully" << std::endl;

}

int main() {
	bip39_tests();
	slip39_tests();
	return 0;
}
