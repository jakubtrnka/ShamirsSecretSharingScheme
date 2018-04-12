#ifndef RIJNDAEL
#define RIJNDAEL

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <array>

/**
 * Galois field F2^8.
 * With polynom x^8 + x^4 + x^3 + x + 1
 * 
 * field division and inversion uses static multiplication table, which
 * is created the first time the class is instantiated
 */
class GF256
{
	friend std::ostream & operator << ( std::ostream & ost, const GF256 & x );
private:
        uint8_t         element;
	static bool	multiplication_table_initialized;
        static std::array<uint8_t, 255>   powers; // with respect to 0xE5
        static void	group_0XE5_multiplication_table_initialization(); // power table one-time-generation

public:

                GF256();
                GF256(const uint8_t _element);

        bool      operator ==(const GF256 & x) const { return element == x.element; }
        GF256     operator + (const GF256 & x) const { return element ^ x.element; }
        GF256     operator - (const GF256 & x) const { return element ^ x.element; }
        GF256     operator * (const GF256 & x) const ;
        GF256     operator / (const GF256 & x) const ;
        GF256     inversion() const;
        uint8_t   octet() const { return element; }
        GF256     pow(int k) const;
};

#endif
