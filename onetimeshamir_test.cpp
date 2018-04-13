#include <iostream>
#include <cassert>
#include <onetimeshamir.h>
#include <get_insecure_randomness.h>
#include <vector>


int main()
{
	std::vector<uint8_t> some_array(10000);
	pseudo_random_fill(some_array);
	return 0;
}
