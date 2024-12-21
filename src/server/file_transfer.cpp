#include "file_transfer.hpp"

void File_Transfer::letsUploadFile(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string &fileName, size_t fileSize){
    const std::string path = "/home/test/MultiplayerChatServer/assets";

    if (!std::filesystem::exists(path)){
        std::filesystem::create_directory(path);
    }

    std::string fullPath = path + "/" + fileName;

    auto buffer = std::make_shared<std::array<char, 1024>>();
    auto remainingSize = std::make_shared<std::size_t>(fileSize);

    std::ofstream outFile(fullPath, std::ios::binary);

    if (!outFile.is_open()) {
        std::cout << "Failed to open file : " << fullPath << std::endl;
        return;
    }

    std::function<void(const boost::system::error_code, std::size_t)> readHandler;

    readHandler = [socket, buffer, remainingSize, &outFile, &readHandler](const boost::system::error_code errCode, std::size_t size) mutable {
        if (!errCode) {
            outFile.write(buffer->data(), size);
            *remainingSize -= size;

            if (remainingSize > 0) {
                socket->async_read_some(boost::asio::buffer(*buffer), readHandler);
            } 
            else {
                std::cout<<"File Upload Has Been COmpleted." << std::endl;
                outFile.close();
            }
        }
        else {
            std::cout<<" Some Error Occured While receiving File Data : " << errCode.message() << std::endl; 
        }
    };

    socket->async_read_some(boost::asio::buffer(*buffer), readHandler);
}