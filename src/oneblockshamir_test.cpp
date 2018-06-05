#include <oneblockshamir.h>
#include <get_insecure_randomness.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <algorithm>
#include <cstring>

#define DEGREE_GEN 13
#define RECONSTRUCT_FROM_COUNT 14
#define SECRET_GEN 52

//#define HARDCODED
#define GENERATED

/**
 * Tests if some random subsets of generated shares restores original secret
 * or if fails given not enough shares. In case not enough shares were given,
 * incorrect secret is produced instead of error throwing.
 *
 * Test runs either with Hardcoded data or is generated in place.
 */
void test0()
{
	std::vector<Shamir::xy_point> shares;
#ifdef HARDCODED
#undef GENERATED
	// Shamir: threshold = 14
	// secret = 64
	shares.push_back(Shamir::xy_point(1, 193));
	shares.push_back(Shamir::xy_point(2, 155));
	shares.push_back(Shamir::xy_point(3, 57));
	shares.push_back(Shamir::xy_point(4, 64));
	shares.push_back(Shamir::xy_point(5, 20));
	shares.push_back(Shamir::xy_point(6, 12));
	shares.push_back(Shamir::xy_point(7, 66));
	shares.push_back(Shamir::xy_point(8, 43));
	shares.push_back(Shamir::xy_point(9, 38));
	shares.push_back(Shamir::xy_point(10, 16));
	shares.push_back(Shamir::xy_point(11, 3));
	shares.push_back(Shamir::xy_point(12, 141));
	shares.push_back(Shamir::xy_point(13, 123));
	shares.push_back(Shamir::xy_point(14, 90));
	shares.push_back(Shamir::xy_point(15, 139));
	shares.push_back(Shamir::xy_point(16, 186));
	shares.push_back(Shamir::xy_point(17, 200));
	shares.push_back(Shamir::xy_point(18, 108));
	shares.push_back(Shamir::xy_point(19, 207));
	shares.push_back(Shamir::xy_point(20, 150));
	shares.push_back(Shamir::xy_point(21, 47));
	shares.push_back(Shamir::xy_point(22, 179));
	shares.push_back(Shamir::xy_point(23, 226));
	shares.push_back(Shamir::xy_point(24, 99));
	shares.push_back(Shamir::xy_point(25, 13));
	shares.push_back(Shamir::xy_point(26, 66));
	shares.push_back(Shamir::xy_point(27, 192));
	shares.push_back(Shamir::xy_point(28, 185));
	shares.push_back(Shamir::xy_point(29, 50));
	shares.push_back(Shamir::xy_point(30, 16));
#elif defined GENERATED
#undef HARDCODED
	GFpolynomial tmp_pol(SECRET_GEN, DEGREE_GEN);
	for (int i=1; i<256; ++i) {
		shares.push_back(tmp_pol.getShare(i));
	}
#else
#undef RECONSTRUCT_FROM_COUNT
#define RECONSTRUCT_FROM_COUNT 1
	shares.push_back(Shamir::xy_point(0, SECRET_GEN));
#endif

	/**
	 * make 100 experiments
	 * randomly chose 'share_threshold' number of shares and try to reconstruct shared secret
	 */
	int share_threshold = RECONSTRUCT_FROM_COUNT; /// How many shares to take to restore. Too little number won't restore secret correctly
	std::default_random_engine gen(1087282);   // seeding generator with arbitrary value
	std::vector<Shamir::xy_point> reconstr;
#ifdef HARDCODED
	std::cout << "testing hardcoded...\n";
#elif defined GENERATED
	std::cout << "testing generated...\n";
#else
	std::cout << "testing single share...\n";
#endif
	for ( int i=0; i<100; ++i) {
		std::shuffle(shares.begin(), shares.end(), gen);
		std::for_each( shares.begin(), shares.begin() + share_threshold, [&reconstr] (auto ii) {reconstr.push_back(ii);});
		GFpolynomial restored_secret(reconstr);
#ifdef HARDCODED
		assert(restored_secret.getSecret() == 0x40);
#else
		assert(restored_secret.getSecret() == SECRET_GEN);
#endif
		reconstr.clear();
	}

}

void test1() {
	std::vector<Shamir::xy_point> shares;
	try {
		GFpolynomial p(shares);
	}
	catch ( const char * s ) {
		assert(0 == std::strcmp(s, "Invalid shamir share set"));
	}
	shares.push_back(Shamir::xy_point(142, 49));
	GFpolynomial p2(shares);
	assert(p2.getSecret() == 49);
}

int main() {
	test0();
	test1();
	std::cout << "ok" << std::endl;
	return 0;
}
