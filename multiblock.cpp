#include <multiblock.h>
#include <sha256.h>

#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <endian.h>

std::vector<uint8_t> checksummed16::serialize() const {
	CSHA256 h;
	std::array<uint8_t, 32> hash;
	std::vector<uint8_t> output(*this);

	h.Write(this->data(), this->size());
	h.Finalize(hash.data());
	h.Reset();

	uint16_t tmp;
	uint8_t * tmps = reinterpret_cast<uint8_t*>(&tmp);
	tmp = static_cast<uint16_t>(hash[0]);
	tmp <<= ((8 - (data_bits % 8)) % 8);
	tmp = htobe16(tmp);
	output.back() |= tmps[0];
	output.push_back(tmps[1]);
	//std::cout << std::hex << "<" << std::setfill('0') << std::setw(2) << (int)hash[0] << std::setfill('0') << std::setw(2) << (int)hash[1] << ">\n";

	tmp = static_cast<uint16_t>(hash[1]);
	tmp <<= ((8 - (data_bits % 8)) % 8);
	tmp = htobe16(tmp);
	output.back() |= tmps[0];
	output.push_back(tmps[1]);
	return output;
}

bool checksummed16::deserialize() {
	if (this->size() < 3) return false;
	std::vector<uint8_t> tmpv(*this);
	uint32_t tmpchecksum;
	uint8_t * tmparray = reinterpret_cast<uint8_t*>(&tmpchecksum);
	auto it = tmpv.rbegin();
	tmparray[3] = *it;
	++it;
	tmparray[2] = *it;
	++it;
	tmparray[1] = *it;
	tmparray[0] = 0;
	tmpchecksum = be32toh(tmpchecksum);
	tmpchecksum >>= (8 - (data_bits % 8)) % 8;
	tmpchecksum = htobe32(tmpchecksum);
	tmpv.pop_back();
	tmpv.pop_back();
	const uint8_t masq = 255 << (8 - (data_bits % 8)) % 8;
	tmpv.back() &= masq;
	CSHA256 sha;
	uint8_t checkhashverify[32];
	sha.Write(tmpv.data(), tmpv.size());
	sha.Finalize(checkhashverify);
	sha.Reset();
	if (checkhashverify[0] != tmparray[2]) return false;
	if (checkhashverify[1] != tmparray[3]) return false;

	this->pop_back();
	this->pop_back();
	this->back() &= masq;
	data_bits -= 16;

	return true;
}

checksummed16::checksummed16(const std::vector<uint8_t> & data) : std::vector<uint8_t>(data) {
	data_bits = data.size() * 8;
}

checksummed16::checksummed16(const std::vector<uint8_t> & data, size_t bits) : std::vector<uint8_t>(data) {
	size_t bytes = bits / 8 + ((bits % 8 == 0) ? 0 : 1); 
	if (data.size() * 8 < bits) throw "Too short data supplied for checksum";
	this->resize(bytes); 
	const uint8_t masq_last = 255u << (8 - (bits % 8)) % 8;
	this->back() &= masq_last;
	data_bits = bits;	
}

share::share(const std::vector<uint8_t> & p) {
	if (p.size() < 5) throw "invalid share packet";
	checksummed16 payload(p, p.size() * 8 - 6);
	if (!payload.deserialize()) throw "checksum verification failed";
	uint16_t overflower(0);
	uint8_t * tmp_ar = reinterpret_cast<uint8_t *>(&overflower);
	tmp_ar[0] = payload[0];
	tmp_ar[1] = payload[1];
	overflower = be16toh(overflower);
	index = (overflower >> 11) + 1;
	threshold = ((overflower >> 6) & 31) + 1;
	for (size_t i = 2; i < payload.size(); ++i) {
		tmp_ar[0] = payload[i - 1];
		tmp_ar[1] = payload[i];
		overflower = be16toh(overflower);
		overflower <<= 2;
		overflower = htobe16(overflower);
		data.push_back(tmp_ar[0]);
	}
}

checksummed16 Shamir::make_share(uint16_t index, uint16_t threshold, const std::vector<uint8_t> & data) {
	uint16_t tmp_it = (index - 1) << 11;
	tmp_it |= ((threshold - 1) & 31)  << 6;
	tmp_it = htobe16(tmp_it);
	uint8_t * tmp_array = reinterpret_cast<uint8_t *>(& tmp_it);
	std::vector<uint8_t> wrapper;
	wrapper.push_back(tmp_array[0]);
	wrapper.push_back(tmp_array[1]);
	for (auto i: data) {
		tmp_it = htobe16(static_cast<uint16_t>(i) << 6);
		wrapper.back() |= tmp_array[0];
		wrapper.push_back(tmp_array[1]);
	}
	return checksummed16(wrapper, data.size()*8 + 10);
}
