#include <rijndael.h>

namespace {
	/// Russian peasant multiplication algorithm
	uint8_t russian_peasant_multiplication(uint8_t x, uint8_t y) {
		uint8_t result(0);
		while (x && y) {
			if (y & 1) result ^= x;
			if (x & 0x80) x = (x << 1) ^ 0x11b;
			else x <<= 1;
			y >>= 1;
		}
		return result;
	}
} // anonymous namespace

/// static members initialization
bool GF256::multiplication_table_initialized(false);
std::array<uint8_t, 255> GF256::powers;

/**
 * Generation of Rijndael field multiplication table with generator 0xe5
 * The table is initialized the first time class is instantiated.
 */
void GF256::group_0XE5_multiplication_table_initialization() {
	/// initialize the static array
        const uint8_t generator(0xe5);
	uint8_t tmp_field_element(0x01);
	for (int i=0; i<255; ++i) {
		powers[i] = tmp_field_element;
		tmp_field_element = russian_peasant_multiplication(tmp_field_element, generator);
	}
	if ( tmp_field_element != 1 ) throw "Rijndael's field table construction failed failed";
}

GF256::GF256() : element(0) {
        if (!multiplication_table_initialized) {
                group_0XE5_multiplication_table_initialization();
                multiplication_table_initialized = true;
        }
}

GF256::GF256(const uint8_t _element) : element(_element) {
        if (!multiplication_table_initialized) {
                group_0XE5_multiplication_table_initialization();
                multiplication_table_initialized = true;
        }
}

std::ostream & operator << ( std::ostream & ost, const GF256 & x ) {
	auto fmtflags = ost.flags();
	ost << std::setfill('0') << std::setw(2) << std::hex << (int) x.element;
	ost.flags(fmtflags);
	return ost;
}

GF256	GF256::operator * (const GF256 & x) const {
        return GF256(russian_peasant_multiplication(element, x.element));
}

GF256	GF256::inversion() const {
        if ( element == 0 ) throw "Zero element can't be inverted\n";
        uint8_t dlog;
        bool    dlogSet(false);
        for (uint8_t i=0; i<255; ++i) {
                if ( powers[i] == element ) {
                        dlogSet = true;
                        dlog = i;
			break;
                }
        }
        if (!dlogSet) throw "some error in field inversion: DLog hasn't been found\n";
        return GF256(powers[(255-dlog) % 255]);
}

/**
 * Field division by DLog lookup-table
 */
GF256   GF256::operator / (const GF256 & x) const
{
        if ( x.element == 0 ) throw "Division by zero";
        if ( element == 0 ) return GF256(0);

        uint8_t log1(0), log2(0);
        bool log1set(false), log2set(false);
        for (size_t i=0; i<powers.size(); ++i) {
                if (element == powers[i]) {
                        log1set = true;
                        log1 = i;
                }
                if (x.element == powers[i]) {
                        log2set = true;
                        log2 = i;
                }
                if (log1set && log2set) break;
        }
        if ( ! (log1set && log2set) ) throw "some error by field inversion. DLog not found.";
        return GF256(powers[(log1 - log2 + 255) % 255]) ;
}

GF256 GF256::pow(int k) const {
        if ( element == 0 ) return *this;
        if ( k == 0 ) return GF256(1);
        GF256 output(1); 
        for ( int i=128; i>0; i>>=1 ) {
                output = output * output;
                if (k & i) output = output * (*this);
        }
        return output;
}
