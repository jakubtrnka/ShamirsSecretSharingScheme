#include <multiblock.h>
#include <sha256.h>

#include <iostream>
#include <string>
#include <cassert>
#include <iomanip>

void check_big() {
	int order(29);
	std::vector<uint8_t> data(1 << order);
	CSHA256 h;
	for (size_t i = 0; i < data.size(); ++i) {
		data[i] = (uint8_t) (i * 3 % 256);
	}
	checksummed16 collected(Shamir::make_share(11, 30, data));
	std::vector<uint8_t> serialized = collected.serialize();

	assert(collected.getLength() + 16 == 8 * (1ul << order) + 26); /// check container size: 10 bits index + threshold, 16 bits checksum, rest is data

	checksummed16 checksumcheck(serialized, collected.getLength() + 16);
	assert(checksumcheck.deserialize()); //checksummed data validated and trimmed

	share deserialized(serialized);
	assert(deserialized.data == data); //share deserialization ok
}

void check_parser() {
	/// last 6 bits of serialized share are just array padding, so verification should not depend on those bits;
	for (uint8_t ii = 0; ii < 64; ++ii) {
		uint8_t tmp[] = {0xbe, 0xcf, 0xcf, 0xcf, 0xe1, 0xc8, (uint8_t)((0x40 + ii) % 256)};
		uint8_t tmp_data[] = {63, 63, 63};
		std::vector<uint8_t> data(std::begin(tmp), std::end(tmp)), orig(std::begin(tmp_data), std::end(tmp_data));
		share sss(data); // parsed?
		assert(sss.index == 24);
		assert(sss.threshold == 28);
		assert(sss.data == orig);
	}

	/// variation of any other bits, nevertheless, should result in failure
	for (int ii = 64; ii < 256; ++ii) {
		uint8_t tmp[] = {0xbe, 0xcf, 0xcf, 0xcf, 0xe1, 0xc8, (uint8_t)((0x40 + ii) % 256)};
		uint8_t tmp_data[] = {63, 63, 63};
		std::vector<uint8_t> data(std::begin(tmp), std::end(tmp)), orig(std::begin(tmp_data), std::end(tmp_data));
		try {
			share sss(data);
		} catch (const char *s) {
			if(std::string(s) == std::string("checksum verification failed"))
			{
				continue; //correctly not-parsed\n";
			}
		}
		assert(false); /// this should be never reached
	}
}

int main() {
	try {
		check_big();
		check_parser();
	} catch (const char *s) {
		std::cout << s << std::endl;
	}
	return 0;
}
