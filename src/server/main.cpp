#include "server.hpp"

int main(){

    try {
        std::string addr = "127.0.0.1";
        short portNo = 8000;
        server server(addr, portNo);
        server.run();
    }
    catch (const std::exception& er) {
        std::cout<<"\n Some Error Occured : " << er.what() << std::endl;
    }

    return 0;
}