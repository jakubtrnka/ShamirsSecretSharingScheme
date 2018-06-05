#include <get_insecure_randomness.h>
#include <fstream>

void pseudo_random_fill(std::vector<uint8_t> & chunk)
{
	std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
	if (urandom) {
		urandom.read(reinterpret_cast<char *>(chunk.data()), chunk.size() * sizeof(chunk.back()));
		if (!urandom) throw "Failed to generate randomness";
		urandom.close();
	} else throw "Failed to open /dev/urandom";

}
