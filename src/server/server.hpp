#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <mutex>

class server
{
private:
    void acceptConnections();

    std::mutex client_mutex;
    boost::asio::io_context _ioContext;
    boost::asio::ip::tcp::acceptor _accepter;
    std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> _sockets;
public:
    server(const std::string &addr, unsigned short port);
    void run();

    void readFromClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void letsUploadFile(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string &fileName, size_t fileSize);
    void letsDownloadFile(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string &fileName);
};

#endif