#include "Hasher.h"
#include <boost/crc.hpp>
#include <string>
#include <cstdint>
#include <memory>

std::string CRC32Hasher::hash(const char* data, size_t size) const {
    boost::crc_32_type result;
    result.process_bytes(data, size);
    uint32_t h = result.checksum();
    return std::string(reinterpret_cast<const char*>(&h), sizeof(h));
}

std::unique_ptr<Hasher> CRC32Hasher::clone() const {
    return std::make_unique<CRC32Hasher>();
}