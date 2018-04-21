#include <oneblockshamir.h>
#include <get_insecure_randomness.h>
#include <rijndael.h>

#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>


GFpolynomial::GFpolynomial( uint8_t _secret, int _degree ) : m_degree(_degree)
{
	if ( m_degree < 1 ) throw "invalid polynomial degree";
	m_coefficients.resize(m_degree + 1);
	pseudo_random_fill(m_coefficients);
	m_coefficients[0] = _secret;
	//for ( auto i: m_coefficients) std::cout << std::setw(3) << (int) i << ' ';
	std::cout << '\n';
}

/**
 * Polynomial is constructed by Lagrange interpolation from points (x_i, y_i)
 * L(x) = Sum(y_j * l_j(x)); j = 0, 1, ..., k
 * l_j(x) = Product((x - x_m)/(x_j - x_m)) j = 0, 2, ..., j-1, j+1, ..., k
 *
 * simplified version only calculates L_(0)
 */
GFpolynomial::GFpolynomial(const std::vector<Shamir::xy_point> & _points, bool simplified)
{
	if (simplified == false) throw "Not implemented yet";
	GF256 result(0);
	for (unsigned j = 0; j < _points.size(); ++j)
	{
		GF256 tmp(1);
		for (unsigned m = 0; m < _points.size(); ++m)
		{
			if (m == j) continue;
			tmp = tmp * GF256(_points[m].first) / (GF256(_points[j].first) - GF256(_points[m].first));
		}
		result = result + (GF256(_points[j].second) * tmp);
	}
	m_degree = 0;
	std::cout << result;
	m_coefficients.push_back(result.octet());
}

Shamir::xy_point GFpolynomial::getShare(int _index) const
{
	if (_index > 255) throw "invalid share index";
	//m_coefficients;
	GF256 result(0u);
	const GF256 index(static_cast<uint8_t>(_index));

	for (int i = 0; i <= m_degree; ++i)
	{
		result = result + index.pow(i) * GF256(m_coefficients[i]);
		std::cout << " + (" << index.pow(i) << " * " << GF256(m_coefficients[i]) << ")";
	}
	std::cout << " = " << result << std::endl;
	return Shamir::xy_point(index.octet(), result.octet());
}
