#ifndef ONEBLOCKSHAMIR
#define ONEBLOCKSHAMIR

#include <rijndael.h>

#include <vector>
#include <utility>
#include <cstdint>

namespace Shamir
{
	typedef uint8_t x_coord;
	typedef uint8_t y_coord;
	typedef std::pair<x_coord, y_coord> xy_point;
} // Shamir namespace

/**
 * polynomial of the form a0 + a1 x + a2 x^2 ... with 
 * [ a0, a1, a2, ...] stored in vector m_coefficients
 */
class GFpolynomial
{
	private:
		//int m_degree;
		std::vector<uint8_t> m_coefficients; 
		void polyInit(size_t _degree);
	public:
		GFpolynomial(uint8_t _secret, int _degree); // creates new random polynomial containing secret _secret;
		GFpolynomial(const std::vector<Shamir::xy_point> & _points, bool simplified = true); //restores polynomial from points
		Shamir::xy_point	getShare(int _index) const;	// creates point-representation of Shamir's share at point x = _index
		uint8_t			getSecret() const;
};


#endif
