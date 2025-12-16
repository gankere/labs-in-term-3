#pragma once
#include <memory>
#include <string>

class Hasher {
public:
    virtual ~Hasher() = default;
    virtual std::string hash(const char* data, size_t size) const = 0;
    virtual std::unique_ptr<Hasher> clone() const = 0;
};

class CRC32Hasher : public Hasher {
public:
    std::string hash(const char* data, size_t size) const override;
    std::unique_ptr<Hasher> clone() const override;
};

class MD5Hasher : public Hasher {
public:
    std::string hash(const char* data, size_t size) const override;
    std::unique_ptr<Hasher> clone() const override;
};