#pragma once
#include <string>
#include <vector>
#include <stdexcept>

using byte = unsigned char;

struct ArchiveDecompressException : std::runtime_error {
    ArchiveDecompressException(const std::string& what) : std::runtime_error(what) {};
};
struct ArchiveCompressException : std::runtime_error {
    ArchiveCompressException(const std::string& what) : std::runtime_error(what) {};
};

struct IArchive
{
    virtual void Compress() = 0;
    virtual void Decompress() = 0;
    virtual void SaveTo(const std::string& dest_path) const = 0;
    virtual ~IArchive() {};
};

class ZlibArchive : public IArchive
{
private:
    std::vector<byte> data;
    constexpr static int initial_expand_rate = 4;
public:
    ZlibArchive(const std::string& path);
    ZlibArchive(std::vector<byte>&& _data);
    void Compress() override;
    void Decompress() override;
    void SaveTo(const std::string& dest_path) const override;
    std::vector<byte>&& ExtractData() &&;
};