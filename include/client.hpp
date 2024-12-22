#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <filesystem>
#include <fstream>
#include <thread>
#include <string>

using boost::asio::ip::tcp;

std::string colorText(const std::string& text, const std::string& colorCode);

class Client
{
private:
    boost::asio::io_context _ioContext;
    tcp::socket _socket;
    bool _Connected;
    std::string _userName;

    void handleInput();
    void sendMSG(const std::string& message);
    void receiveMSG();
    void uploadFile(const std::string& filePath);
    void downloadFile(const std::string& filePath);
    void printError(const std::string& errMSG);

public:
    void start();
    Client(const std::string& addr, unsigned short port, const std::string& userName);
};

#endif // CLIENT_HPP