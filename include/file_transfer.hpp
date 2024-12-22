#ifndef FILE_TRANSER_HPP
#define FILE_TRANSFER_HPP

#include <boost/asio.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <memory>

class File_Transfer {
public:
    void letsUploadFile(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string &fileName, size_t fileSize);
    void letsDownloadFile(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string &fileName);
};

#endif // FILE_TRANSFER_HPP