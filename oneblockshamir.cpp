#include <oneblockshamir.h>
#include <get_insecure_randomness.h>
#include <rijndael.h>

#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>


void GFpolynomial::polyInit(size_t _degree) {
	m_coefficients.resize(_degree + 1);
	pseudo_random_fill(m_coefficients);
}

GFpolynomial::GFpolynomial(uint8_t _secret, int _degree) {
	if (_degree < 0 || _degree > 255) throw "invalid polynomial degree";
	polyInit(_degree);
	m_coefficients[0] = _secret;
}

uint8_t GFpolynomial::getSecret() const {
	return m_coefficients[0];
}

/**
 * Polynomial is constructed by Lagrange interpolation through points (x_i, y_i)
 * L(x) = Sum(y_j * l_j(x)); j = 0, 1, ..., k
 * l_j(x) = Product((x - x_m)/(x_j - x_m)) j = 0, 2, ..., j-1, j+1, ..., k
 *
 * simplified version only calculates L_(0), i. e. secret, and generates brand new
 * random polynomial of corresponding degree
 */
GFpolynomial::GFpolynomial(const std::vector<Shamir::xy_point> & _points, bool simplified) {
	if (simplified == false) throw "Not supported yet";
	if (_points.size() < 1) throw "Invalid shamir share set";
	GF256 result(0);
	for (size_t j = 0; j < _points.size(); ++j)
	{
		GF256 tmp(1);
		for (size_t m = 0; m < _points.size(); ++m)
		{
			if (m == j) continue;
			tmp = tmp * GF256(_points[m].first) / (GF256(_points[j].first) - GF256(_points[m].first));
		}
		result = result + (GF256(_points[j].second) * tmp);
	}
	polyInit(_points.size() - 1); /// _points.size() == 'polynomial degree + 1'
	m_coefficients[0] = result.octet();
}

Shamir::xy_point GFpolynomial::getShare(int _index) const {
	if (_index > 255 || _index < 1) throw "invalid share index";

	GF256 result(0u);
	const GF256 index(static_cast<uint8_t>(_index));

	for (size_t i = 0; i < m_coefficients.size(); ++i)
	{
		result = result + index.pow(i) * GF256(m_coefficients[i]);
	}
	return Shamir::xy_point(index.octet(), result.octet());
}
