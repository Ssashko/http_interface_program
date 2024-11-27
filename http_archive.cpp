#include "http_archive.hpp"
#include "archive.hpp"

#include <random>
#include <iostream>


void Http_interface_archive_handler::Compress(http::request<http::vector_body<unsigned char>>& request, http::response<http::vector_body<unsigned char>>& response)
{

    ZlibArchive arh(std::move(request).body());
    arh.Compress();
    auto data = std::move(arh).ExtractData();

    http::file_body::value_type file_body;

    response.result(http::status::ok);
    response.set(http::field::content_type, "application/octet-stream");
    response.content_length(data.size());
    response.body() = std::move(data);
}

void Http_interface_archive_handler::Decompress(http::request<http::vector_body<unsigned char>>& request, http::response<http::vector_body<unsigned char>>& response)
{
    ZlibArchive arh(std::move(request).body());
    arh.Decompress();
    auto data = std::move(arh).ExtractData();

    http::file_body::value_type file_body;

    response.result(http::status::ok);
    response.set(http::field::content_type, "application/octet-stream");
    response.content_length(data.size());
    response.body() = std::move(data);
}