#include "server.hpp"

using namespace boost::asio::ip;

server::server(const std::string &addr, unsigned short port) : _accepter(_ioContext, tcp::endpoint(make_address(addr), port)){
    std::cout<<"\n Server has been started on : " << addr << " and port is : " << port << std::endl;
}

void server::readFromClient(std::shared_ptr<tcp::socket> socket){
    auto buffer = std::make_shared<std::array<char, 1024>>();

    socket->async_read_some(boost::asio::buffer(*buffer), [this, socket, buffer](boost::system::error_code errCode, size_t bytes){
            if (!errCode){
                std::string message(buffer->data(), bytes);
                std::cout<<"\n Message From Client : " << message << std::endl;

                {
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
                
                std::cerr<<"\n Client Has been disconnected : " << socket->remote_endpoint() << std::endl;

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

            std::cout<<"\n Client has been connnected : " << socket->remote_endpoint() << std::endl;
            readFromClient(socket);
        }
        else {
            std::cout<< "\n Error has been occured : " << errCode.message() << std::endl;

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

