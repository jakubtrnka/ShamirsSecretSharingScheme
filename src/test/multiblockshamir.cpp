#include <multiblockshamir.h>
#include <oneblockshamir.h>
#include <bit_container.h>

namespace Shamir {
	std::vector<uint8_t> reconstruct_secret_raw(const std::vector<std::vector<xy_point>> & raw_shares) {
		std::vector<uint8_t> output;
		size_t min(-1), max(0);
		for (auto && it: raw_shares) {
			min = (it.size() < min) ? it.size() : min;
			max = (it.size() > max) ? it.size() : min;
		}
		if ( min != max) throw "Shares must have the same length";
		for (unsigned j=0; j < min; ++j) {
			std::vector<xy_point> block_share;
			for (unsigned i = 0; i < raw_shares.size(); ++i) {
				block_share . push_back(raw_shares.at(i).at(j));
			}
			output.push_back(GFpolynomial(block_share) . getSecret());
		}
		return output;
	}
	
	std::vector<std::vector<uint8_t>> distribute_secret_raw(const std::vector<uint8_t> & secret, uint16_t count, uint16_t threshold) {
		//bit_container share
		if (count < threshold) throw "Number of shares must be greater or equal to the reconstruction threshold";
		std::vector<std::vector<uint8_t>> output(count);
		for (auto it: secret) {
			GFpolynomial m(it, threshold - 1);
			for (int i = 1; i <= count; ++i) output.at(i-1) . push_back(m.getShare(i));
		}
		return output;
	}
} // Shamir
