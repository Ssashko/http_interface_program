#pragma once

#include <boost/beast/http.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

class Http_interface_archive_handler
{
public:
    static void Compress(http::request<http::vector_body<unsigned char>>& request, http::response<http::vector_body<unsigned char>>& response);
    static void Decompress(http::request<http::vector_body<unsigned char>>& request, http::response<http::vector_body<unsigned char>>& response);
};