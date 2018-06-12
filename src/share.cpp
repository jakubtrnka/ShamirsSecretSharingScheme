#include <share.h>
#include <bit_container.h>
#include <multiblockshamir.h>
#include <sha256.h>

#include <set>
#include <endian.h>
#include <exception>

using namespace Shamir;

namespace {
	union overflower {
		uint8_t array[2];
		uint16_t number;
	};

	bit_container mkshare(uint16_t index, uint16_t threshold, const std::vector<uint8_t> & dat) {
		bit_container output;
		overflower hdr;
		hdr.number = ((--index) << 11) + ((--threshold) << 6);
		hdr.number = htobe16(hdr.number);
		std::vector<uint8_t> tmp;
		tmp.push_back(hdr.array[0]);
		tmp.push_back(hdr.array[1]);
		output.push_n(tmp, 10);
		output.push_n(dat, dat.size() * 8);
		return output;
	}
	
	std::vector<uint8_t> parse_share(uint16_t & index, uint16_t & threshold, bit_container & share) {
		if ((share.get_height() - 10) % 8) throw "Share does not contain correct amount of data";
		std::vector<uint8_t> output;

		share.pop_n(output, share.get_height() - 10);
		std::vector<uint8_t> tmp_it;
		share.pop_n(tmp_it, 5);
		threshold = (tmp_it.at(0) >> 3) + 1;
		share.pop_n(tmp_it, 5);
		index = (tmp_it.at(0) >> 3) + 1;
		return output;
	}
	
	bit_container & append_checksum(bit_container & share) {
		std::vector<uint8_t> md(32);
		CSHA256().Write(share.data(), share.size()).Finalize(md.data());
		share.push_n(md, 16);
		return share;
	}
	
	bool pop_verify_checksum(bit_container & share) {
		std::vector<uint8_t> checksum;
		try {
			share.pop_n(checksum, 16);
		} catch (const std::exception & e) {
			return false;
		}
		std::vector<uint8_t> md(32);
		CSHA256().Write(share.data(), share.size()).Finalize(md.data());
		checksum[0] ^= md[0];
		checksum[1] ^= md[1];
		checksum[0] |= checksum[1];
		return checksum[0] == 0x00;
	}
} // anonymous

namespace Shamir {
	// not true: /// does not involve checking master_secret_checksum. Only verifies chare_checksums
	std::vector<uint8_t> reconstruct_secret_slip(const std::vector<std::vector<uint8_t>> & raw_shares) {
		std::vector<std::vector<xy_point>> raw_share_data;
		std::set<uint8_t> check_unique_indeces, check_same_thresholds;
		for (auto &it: raw_shares) {
			uint16_t index, threshold;
			bit_container tmp_share(it);
			if (tmp_share.size() < 6) throw "Invalid share";
			tmp_share.pop_n((tmp_share.size() * 8 - 42) % 32); // assume secret is multiple of 32 bits
			if (!pop_verify_checksum(tmp_share)) throw "Invalid share"; //verify if last 16 bits is checksum
			std::vector<xy_point> tmp_xy_holder;
			for (auto jt: parse_share(index, threshold, tmp_share)) tmp_xy_holder.push_back(xy_point((uint8_t)index, jt));
			check_unique_indeces.insert(index);
			check_same_thresholds.insert(threshold);
			raw_share_data.push_back(tmp_xy_holder);
		}
		if (check_unique_indeces.size() != raw_shares.size()) throw "Indices was not unique within shares";
		if (check_same_thresholds.size() != 1u) throw "Thresholds must be same for all shares";
		if (check_same_thresholds.find(raw_shares.size()) == check_same_thresholds.end()) throw "Threshold must be equal to the number of shares supplied";
		bit_container output(reconstruct_secret_raw(raw_share_data));
		if (!pop_verify_checksum(output)) throw "Inconsistent shares: master checksum is invalid";
		return output;
	}
	
	// not true: /// does not append master_secret_checksum. Only appends share_checksums
	std::vector<bit_container> distribute_secret_slip(const std::vector<uint8_t> & secret, uint16_t count, uint16_t threshold) {
		if (secret.size() % 4 != 0) throw "Secret must be multiple of 32 bits";
		auto tmp_scr = bit_container(secret);
		auto raw_shares = distribute_secret_raw(append_checksum(tmp_scr), count, threshold);
		std::vector<bit_container> output;
		for (unsigned i = 1; i <= raw_shares.size(); ++i) {
			auto tmp_cont = mkshare(i, threshold, raw_shares.at(i-1));
			output.push_back(append_checksum(tmp_cont));
		}
		return output;
	}
} // Shamir
