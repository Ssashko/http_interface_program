#include "archive.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <zlib.h>

void ZlibArchive::Compress()
{
    uLongf compressed_size = compressBound(data.size());
    std::vector<byte> compressed_data(compressed_size);

    int res = compress(compressed_data.data(), &compressed_size, data.data(), data.size());
    if (res == Z_OK) {
        compressed_data.resize(compressed_size);
        data = std::move(compressed_data);
    } else {
        throw ArchiveCompressException("unable to compress. Error code: " + std::to_string(res));
    }
}
ZlibArchive::ZlibArchive(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if(file.is_open()) {
        size_t file_size = file.tellg();
        data.resize(file_size);
        file.seekg(0, std::ios_base::beg);
        file.read(reinterpret_cast<char*>(data.data()), file_size);
        file.close();
    } else {
        throw std::ios_base::failure("unable to open the file: " + path);
    }
}

void ZlibArchive::Decompress()
{
    int expand_rate = initial_expand_rate;
    int res = Z_OK;
    std::vector<byte> decompressed_data;
    uLongf decompressed_size = 0;
    do
    {
        decompressed_size = data.size() * expand_rate;
        decompressed_data.resize(decompressed_size);
        expand_rate *= 2;
        res = uncompress(decompressed_data.data(), &decompressed_size, data.data(), data.size());
    } while (res == Z_BUF_ERROR);
    
    if (res == Z_OK) {
        decompressed_data.resize(decompressed_size);
        data = std::move(decompressed_data);
    } else {
        throw ArchiveDecompressException("unable to decompress. Error code: " + std::to_string(res));
    }
}
void ZlibArchive::SaveTo(const std::string& dest_path) const
{
    std::ofstream file(dest_path, std::ios::binary);

    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
    }
    else
    {
        throw std::ios_base::failure("unable to open file for writing: " + dest_path);
    }
}


std::vector<byte>&& ZlibArchive::ExtractData() &&
{
    return std::move(data);
}

ZlibArchive::ZlibArchive(std::vector<byte>&& _data) : data(std::move(_data))
{}