#include <get_insecure_randomness.h>
#include <chrono>
#include <random>
#include <iostream>

void pseudo_random_fill(std::vector<uint8_t> & chunk)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<uint8_t> distribution(0, 255);
	for (auto && i : chunk)
	{
		i = distribution(generator);
	}
}
