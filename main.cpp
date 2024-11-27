#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>
#include <string>

#include "http_archive.hpp"

using boost::asio::ip::tcp;

void handle_request(http::request<http::vector_body<unsigned char>>& request, http::response<http::vector_body<unsigned char>>& response)
{
    if (request.method() == http::verb::post && request.target() == "/compress")
        Http_interface_archive_handler::Compress(request, response);
    else if (request.method() == http::verb::post && request.target() == "/decompress")
        Http_interface_archive_handler::Decompress(request, response); 
    else
        response.result(http::status::not_found);
    response.prepare_payload();
}

void handle_session(tcp::socket& socket) {

    beast::flat_buffer buffer;
    http::request<http::vector_body<unsigned char>> request;
    http::read(socket, buffer, request);
    http::response<http::vector_body<unsigned char>> response;
    handle_request(request, response);
    http::write(socket, response);
}

int main(int argc, char* argv[])
{
    const int port = std::atoi(argc == 2 ? argv[1] : DEFAULT_PORT);
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
        std::cout << "Archive http interface program is running on http://localhost:" << port << '\n';

        while(true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            handle_session(socket);
        }
    }
    catch(const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }

    return 0;
}