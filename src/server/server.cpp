#include "server.hpp"
#include "file_transfer.hpp"

using namespace boost::asio::ip;

File_Transfer fileTransfer;

server::server(const std::string &addr, unsigned short port) : _accepter(_ioContext, tcp::endpoint(make_address(addr), port)){
    std::cout<<"Server has been started on : " << addr << " and port is : " << port << std::endl;
}

void server::readFromClient(std::shared_ptr<tcp::socket> socket){
    auto buffer = std::make_shared<std::array<char, 1024>>();

    socket->async_read_some(boost::asio::buffer(*buffer), [this, socket, buffer](boost::system::error_code errCode, size_t bytes){
            if (!errCode){
                std::string message(buffer->data(), bytes);
                std::cout<<"Message From Client : " << message << std::endl;

                if (message.find("Upload:", 0) == 0) {
                    std::istringstream iss(message.substr(7));
                    std::string fileName;
                    size_t fileSize;
                    iss >> fileName >> fileSize;
                    std::cout<<"starting to upload file : "<< fileName << std::endl;
                    fileTransfer.letsUploadFile(socket, fileName, fileSize);
                } 
                else if (message.find("Download:", 0) == 0) {
                    std::string fileName = message.substr(9);
                    std::cout<<"starting to download file : "<< fileName << std::endl;
                    fileTransfer.letsDownloadFile(socket, fileName);
                }
                else {
                    std::lock_guard<std::mutex> lock(client_mutex);

                    for (auto &other : _sockets){
                        if (other != socket){
                            auto send_buffer = std::make_shared<std::string>(message);

                            boost::asio::async_write(*other, boost::asio::buffer(*send_buffer), [send_buffer](boost::system::error_code errCode, size_t bytes) {});
                        }
                    }
                }

                readFromClient(socket);
            }
            else {
                
                std::cerr<<"Client Has been disconnected : " << socket->remote_endpoint() << std::endl;

                {
                    std::lock_guard<std::mutex> lock(client_mutex);
                }
            }
    });
}

void server::acceptConnections(){
    auto socket = std::make_shared<tcp::socket>(_ioContext);

    _accepter.async_accept(*socket, [this, socket](boost::system::error_code errCode){
        if (!errCode){
            
            {
                std::lock_guard<std::mutex> lock(client_mutex);
                _sockets.push_back(socket);
            }

            std::cout<<"Client has been connnected : " << socket->remote_endpoint() << std::endl;
            readFromClient(socket);
        }
        else {
            std::cout<< "Error has been occured : " << errCode.message() << std::endl;

            {
                std::lock_guard<std::mutex> lock(client_mutex);
                _sockets.erase(std::remove(_sockets.begin(), _sockets.end(), socket), _sockets.end());
            }
        }

        acceptConnections();
    });
}

void server::run(){
    acceptConnections();
    _ioContext.run();
}

