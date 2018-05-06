#include <iostream>
#include <set>
#include <cassert>

#include <rijndael.h>

bool test_division_uniqueness() {
	for ( unsigned i = 1; i<=255; ++i ) {
		GF256 x(i);
		std::set<uint8_t> tmp_div;
		std::pair<std::set<uint8_t>::iterator, bool> present;
		for ( unsigned j = 1; j<=255; ++j ) {
			try {
				present = tmp_div.insert( (x / GF256(j)).octet() );
			} catch ( const char * s ) {
				return false;
			}
			if (present.second==false)
				return false;
		}
	}
	return true;
}

bool test_pow(const GF256 & e, int i) {
	GF256 tmp(1);
	for ( int j=0; j<i; ++j) {
		tmp = tmp * e;
	}
	return e.pow(i) == tmp;
}

int main(int argc, const char* argv[]) {
	assert(test_division_uniqueness());
	assert(GF256(1).inversion() == GF256(1));
	for ( int i=1; i<=255; ++i ) {
		assert( (GF256(i) * GF256(1)) == GF256(i));
		assert( (GF256(i) * GF256(0)).octet() == 0);
		assert( (GF256(i) / GF256(i)).octet() == 1);
		assert( GF256(i).pow(3) * GF256(i).inversion() * (GF256(i).inversion().pow(2)) == GF256(1) );
		assert(test_pow( GF256(i), 5));
		assert(test_pow( GF256(33), i));
		assert(! (GF256(14).pow(i) == GF256(0)) );
		assert(! (GF256(94).pow(i) == GF256(0)) );
	}

	assert(GF256(0x7b).pow(1) == GF256(0x7b));

	assert(GF256(0x7b).pow(2) == GF256(0x99));
	assert(GF256(0x7b)*GF256(0x7b) == GF256(0x99));
	assert(GF256(0x7b).pow(3) == GF256(0xb6));
	assert(GF256(0x7b)*GF256(0x7b)*GF256(0x7b) == GF256(0xb6));
	assert(GF256(0x7b).pow(4) == GF256(0xc0));
	assert(GF256(0x7b).pow(5) == GF256(0x20));

	/// test on manualy computed random polynomial of degree 3
	assert(GF256(0x0e) * GF256(0x7b).pow(1) == GF256(0xf4));
	assert(GF256(0x72) * GF256(0x7b).pow(2) == GF256(0x8b));
	assert(GF256(0xa1) * GF256(0x7b).pow(3) == GF256(0x43));
	assert(GF256(0x11) + GF256(0x0e) * GF256(0x7b) + GF256(0x72) * GF256(0x7b).pow(2) + GF256(0xa1) * GF256(0x7b).pow(3) == GF256(0x2d));



	std::cout << "test \'" << argv[0] << "\' completed successfully" << std::endl;

	return 0;
}
