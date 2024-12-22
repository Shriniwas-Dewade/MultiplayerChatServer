#include "file_transfer.hpp"

void File_Transfer::letsUploadFile(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string &fileName, size_t fileSize){
    const std::string path = "/home/test/MultiplayerChatServer/assets";

    if (!std::filesystem::exists(path)){
        std::filesystem::create_directory(path);
    }

    std::string fullPath = path + "/" + fileName;

    auto buffer = std::make_shared<std::array<char, 1024>>();
    auto remainingSize = std::make_shared<std::size_t>(fileSize);

    auto outFile = std::make_shared<std::ofstream>(fullPath, std::ios::binary);
    if (!outFile->is_open()) {
        std::cerr << "Failed to open file: " << fullPath << std::endl;
        return;
    }
    else {
        std::cout <<  "File opened successfully: "  << fullPath << std::endl;
    }

    auto readHandler = std::make_shared<std::function<void(boost::system::error_code, std::size_t)>>();

    *readHandler = [socket, buffer, remainingSize, outFile, readHandler, fileName](const boost::system::error_code errCode, std::size_t size) mutable {
        if (!errCode) {

            std::cout<<"Data received of file " << fileName << " data : " << std::string(buffer->data(), size) << std::endl;

            outFile->write(buffer->data(), size);

            if (!outFile->good()) {
                std::cerr<< "Errro occured while writting data to file" << std::endl;
            }

            if (size > *remainingSize) {
                std::cerr << "Received more data than given size" << std::endl;
                return;
            }

            *remainingSize -= size;

            std::cout<<"File uploading remained of : " << *remainingSize << std::endl;

            if (*remainingSize > 0) {
                socket->async_read_some(boost::asio::buffer(*buffer), *readHandler);
            } 
            else {
                std::cout<<"File Upload Has Been Completed." << std::endl;
                outFile->close();
            }
        }
        else {
            std::cout<<" Some Error Occured While receiving File Data : " << errCode.message() << std::endl;
            outFile->close();
        }
    };

    socket->async_read_some(boost::asio::buffer(*buffer), *readHandler);
}

void File_Transfer::letsDownloadFile(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string &fileName){
    const std::string path = "/home/test/MultiplayerChatServer/assets";

    std::string fullPath = path + "/" + fileName;

    auto inFile = std::make_shared<std::ifstream>(fullPath, std::ios::binary);

    if (!inFile->is_open()) {
        std::cout << "Failed to open file : " << fullPath << std::endl;
        return;
    }
    else {
        std::cout << "File opened succesfully : " << fullPath << std::endl;
    }

    auto buffer = std::make_shared<std::array<char, 2014>>();

    auto writeHandler = std::make_shared<std::function<void(const boost::system::error_code, std::size_t)>>();

    *writeHandler = [buffer, socket, &inFile, writeHandler, fileName](boost::system::error_code errCode, size_t fileSize) mutable {
        if (!errCode) {
            inFile->read(buffer->data(), buffer->size());

            auto remFileSize = inFile->gcount();

            if (remFileSize > 0) {
                boost::asio::async_write(*socket, boost::asio::buffer(buffer->data(), remFileSize), *writeHandler);
            }
            else {
                std::cout<<"File Downloading has been completed for : " << fileName << std::endl;
            }
        }
        else {
            std::cout<<"Error occured during file downloading : " << errCode.message() << std::endl;
        }
    };

    inFile->read(buffer->data(), buffer->size());
    std::streamsize actualSize = inFile->gcount();

    if (actualSize > 0){
        boost::asio::async_write(*socket, boost::asio::buffer(buffer->data(), actualSize), *writeHandler);
    }
}
