#include <oneblockshamir.h>
#include <get_insecure_randomness.h>
#include <rijndael.h>

#include <vector>
#include <cstdint>
#include <set>

using namespace Shamir;

void GFpolynomial::polyInit(size_t _degree) {
	m_coefficients.resize(_degree + 1);
	pseudo_random_fill(m_coefficients);
}

GFpolynomial::GFpolynomial(uint8_t _secret, int _degree) {
	if (_degree < 0 || _degree > 31) throw "invalid polynomial degree";
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
 * only calculates L = L_(0), i. e. secret, i. e. generates constant function
 */
GFpolynomial::GFpolynomial(const std::vector<xy_point> & _points) {
	if (_points.size() < 1) throw "Invalid shamir share set";
	std::set<uint8_t> check_uniqueness;
	for (auto &it: _points) if (!check_uniqueness.insert(it.first).second) throw "Two shares with same index";
	GF256 result(0);
	for (size_t j = 0; j < _points.size(); ++j) {
		GF256 tmp(1);
		for (size_t m = 0; m < _points.size(); ++m) {
			if (m == j) continue;
			tmp = tmp * GF256(_points[m].first) / (GF256(_points[j].first) - GF256(_points[m].first));
		}
		result = result + (GF256(_points[j].second) * tmp);
	}
	polyInit(_points.size() - 1); /// _points.size() == 'polynomial degree + 1'
	m_coefficients[0] = result.octet();
}

uint8_t GFpolynomial::getShare(int _index) const {
	if (_index > 32 || _index < 1) throw "invalid share index";

	GF256 result(0u);
	const GF256 index(static_cast<uint8_t>(_index));

	for (size_t i = 0; i < m_coefficients.size(); ++i) {
		result = result + index.pow(i) * GF256(m_coefficients[i]);
	}
	return result.octet();
}
