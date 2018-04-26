#include <shamirmulti.h>
#include <oneblockshamir.h>
#include <multiblock.h>

#include <iostream>
#include <array>
#include <vector>

#include <iomanip>

std::vector< std::vector<uint8_t> >  distribute(const std::vector<uint8_t> & msg, unsigned threshold, unsigned sharecount) {
	if (sharecount < threshold) throw "share threshold must be equal or greater than total share count";
	if (threshold == 0) throw "threshold must be greater than zero";
	std::vector<uint8_t> chcksmd(checksummed16(msg).serialize());

	std::vector< std::vector<Shamir::xy_point> > share_coll(chcksmd.size());
	for (auto j=0u; j<chcksmd.size(); ++j) {
		GFpolynomial sharepoly(chcksmd[0], threshold - 1);
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
		std::cout << "<<<" << index + 1 << ", " << threshold << ", ";
		for (auto iii: share_packet.serialize()) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)iii;
		std::cout << ">>>\n";
		final_shares.push_back(share_packet.serialize());
	}
	return final_shares;
}

void f() {
	const uint8_t tmp[] = "this is a very secret message\n";
	std::vector<uint8_t> secretmessage(std::begin(tmp), std::end(tmp));
	auto ii = distribute(secretmessage, 4, 8);
	for (auto j: ii) {
		for (auto i: j) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)i;
		std::cout << std::endl;
	}
}
	//for (auto i: chcksmd) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)i;
