#include <iostream>
#include <cassert>
#include <oneblockshamir.h>
#include <get_insecure_randomness.h>
#include <vector>


int main()
{
	GFpolynomial pokus(13,2);
	std::vector<Shamir::xy_point> cpx;
	cpx.push_back(pokus.getShare(1));
	cpx.push_back(pokus.getShare(2));
	cpx.push_back(pokus.getShare(3));
	//cpx.push_back(pokus.getShare(4));
	//cpx.push_back(pokus.getShare(5));
	GFpolynomial novy(cpx);
	return 0;
}
