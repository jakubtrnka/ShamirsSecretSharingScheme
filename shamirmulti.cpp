#include <shamirmulti.h>
#include <oneblockshamir.h>
#include <multiblock.h>

#include <endian.h>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>

#include <iomanip>

namespace {
	void checkSameLengths(const std::vector< std::vector<uint8_t> > & share_list) {
		for (auto && i: share_list) {
			if (i.size() != share_list[0].size()) throw "Shares do not have same length";
		}
	}

	void extractIndexThreshold(unsigned & ind, unsigned & thr, const std::vector<uint8_t> & data) {
		uint16_t overflower;
		uint8_t * tmp_array = reinterpret_cast<uint8_t *>(&overflower);
		tmp_array[0] = data[0];
		tmp_array[1] = data[1];
		overflower = be16toh(overflower);
		ind = (overflower >> 11) + 1;
		thr = ((overflower >> 6) & 31) + 1;
	}

	void checkDistinctIndicesSameThresholds(const std::vector< std::vector<uint8_t> > & share_list) {
		std::vector<unsigned> indices, thresholds;
		for (const auto & it: share_list) {
			unsigned ind, thr;
			extractIndexThreshold(ind, thr, it);
			indices.push_back(ind);
			thresholds.push_back(thr);
		}
		for (const auto & it: thresholds) {
			if (it != thresholds[0]) throw "Shares do not signal same threshold";
		}

		if (thresholds[0] > thresholds.size()) throw "Not enough shares supplied";

		std::sort(indices.begin(), indices.end());
		for (unsigned i=1; i<indices.size(); ++i) {
			if (indices[i] == indices[i-1]) throw "Share indices are not distinct";
		}
	}
} // anonymous namespace

namespace Shamir {
	std::vector< std::vector<uint8_t> >  distribute(const std::vector<uint8_t> & msg, unsigned threshold, unsigned sharecount) {
		if (sharecount < threshold) throw "share threshold must be equal or greater than total share count";
		if (threshold == 0) throw "threshold must be greater than zero";
		std::vector<uint8_t> chcksmd(checksummed16(msg).serialize());
	
		std::vector< std::vector<Shamir::xy_point> > share_coll(chcksmd.size());
		for (auto j=0u; j<chcksmd.size(); ++j) {
			GFpolynomial sharepoly(chcksmd[j], threshold - 1);
			for (unsigned i = 1; i <= sharecount; ++i) {
				share_coll[j].push_back(sharepoly.getShare(i));
			}
		}
	
		std::vector< std::vector<uint8_t> > final_shares;
		for (auto index = 0u; index < sharecount; ++index) {
			std::vector<uint8_t> tmp_container;
			for (auto i = 0u; i < chcksmd.size(); ++i) {
				if (share_coll[i][index].first != index + 1) throw "inconsistent share indexing";
				tmp_container.push_back(share_coll[i][index].second);
			}
			checksummed16 share_packet = Shamir::make_share(index + 1, threshold, tmp_container);
			final_shares.push_back(share_packet.serialize());
		}
		return final_shares;
	} // distribute func
	
	std::vector<uint8_t> reconstruct(const std::vector<std::vector<uint8_t>> & share_list) {
		checkSameLengths(share_list);
		checkDistinctIndicesSameThresholds(share_list);
	
		std::vector<std::vector<Shamir::xy_point>> plain_shares;
		std::vector<uint16_t> indices;
		for (auto && i: share_list) {
			share tmp_cont(i);
			for (size_t j = 0; j < tmp_cont.data.size(); ++j) {
				while (plain_shares.size() < j+1) plain_shares.push_back(std::vector<Shamir::xy_point>());
				plain_shares[j].push_back(Shamir::xy_point(tmp_cont.index, tmp_cont.data[j]));
			}
		}
		std::vector<uint8_t> secret_cont;
		for (auto && i: plain_shares) {
			GFpolynomial recoveryPoly(i);
			secret_cont.push_back(recoveryPoly.getSecret());
		}
		checksummed16 output(secret_cont);
		if (output.deserialize() == false) throw "Secret message checksum verification failed. Message possibly corrupted.";
		return output;
	} // reconstruct func
} // Shamir namespace
