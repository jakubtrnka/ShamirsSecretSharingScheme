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
	/*
	std::cout << "supplied:   " << std::hex << std::setw(2) << std::setfill('0')
		<< (int) tmparray[2] << " " << (int) tmparray[3]  << std::endl;
	std::cout << "calculated: " << std::hex << std::setw(2) << std::setfill('0')
		<< (int) checkhashverify[0] << " " << (int) checkhashverify[1]  << std::endl;
	*/
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
	size_t bytes = bits/8 + ((bits%8 == 0) ? 0 : 1); 
	if (data.size() * 8 < bits) throw "Too short data supplied for checksum";
	this->resize(bytes); 
	const uint8_t masq_last = 255u << (8 - (bits % 8)) % 8;
	this->back() &= masq_last;
	data_bits = bits;	
}

void f() {
	std::vector<uint8_t> vvv;
	for (int i=0; i<5; ++i) vvv.push_back((uint8_t) 255);
	checksummed16 cont(vvv, 27);

	std::vector<uint8_t> packet(cont.serialize());
	for (auto i: packet) std::cout << std::setw(2) << std::setfill('0') << std::hex << (int) i << '.' ;
	std::cout << std::endl;

	std::cout << "reloading:\n";
	std::vector<uint8_t> vvvv;
	vvvv.push_back(255);
	vvvv.push_back(255);
	vvvv.push_back(255);
	vvvv.push_back(0xf5);
	vvvv.push_back(0xfd);
	vvvv.push_back(0);
	checksummed16 ff(vvvv, 43);
	ff.deserialize();
	std::cout << "reserialization\n"; 
	std::vector<uint8_t> paket(ff.serialize());
	for (auto i: paket) std::cout << std::setw(2) << std::setfill('0') << std::hex << (int) i << '.' ;
}
