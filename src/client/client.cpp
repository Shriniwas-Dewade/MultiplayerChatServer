#include "client.hpp"

std::string colorText(const std::string& text, const std::string& colorCode) {
    return "\033[" + colorCode + "m" + text + "\033[0m";
}

Client::Client(const std::string& addr, unsigned short port, const std::string& userName) : _socket(_ioContext), _Connected(false), _userName(userName) {
    try {
        tcp::resolver resolver(_ioContext);
        auto endPoint = resolver.resolve(addr, std::to_string(port));
        boost::asio::connect(_socket, endPoint);
        _Connected = true;
        std::cout<<"Connected to the server at : " << addr << ":" << std::to_string(port) << " as " << userName << std::endl;
    }
    catch (const std::exception& err) {
        printError("Failed to connect server : " + std::string(err.what()));
    }
}

void Client::printError(const std::string& errorMessage) {
    std::cerr << "[Error] " << errorMessage << std::endl;
}

void Client::start() {
    std::thread receiveMSGS(&Client::receiveMSG, this);
    handleInput();

    if (receiveMSGS.joinable()) {
        receiveMSGS.join();
    }
}

void Client::handleInput() {
    std::string input;

    while (_Connected) {
        std::cout << "\n" << colorText("================= MAIN MENU =================", "34") << std::endl;
        std::cout << colorText("1. SEND     - Send a message to other clients.", "33") << std::endl;
        std::cout << colorText("2. UPLOAD   - Upload a file to the server.", "33") << std::endl;
        std::cout << colorText("3. DOWNLOAD - Download a file from the server.", "33") << std::endl;
        std::cout << colorText("4. EXIT     - Disconnect and exit the client.", "33") << std::endl;
        std::cout << colorText("============================================", "34") << "\n";
        std::cout << colorText("Enter your choice (SEND, UPLOAD, DOWNLOAD, EXIT):", "36") << " ";
        std::getline(std::cin, input);

        if (input.find("EXIT", 0) == 0) {
            std::cout << "Exiting the client..." << std::endl;
            _Connected = false;
            break;
        }
        else if (input.find("SEND", 0) == 0) {
            std::string message;
            std::cout << "\n" << colorText("--- SEND MODE: Type your message. Press Ctrl + E to exit this mode. ---", "34") << "\n";
            
            while (true) {
                std::cout << colorText("You:", "32") << " ";
                getline(std::cin, message);

                if (message == "\x05") {
                    break;
                }

                sendMSG(message);
            }
        }
        else {
            printError("Invalid command. Try SEND, UPLOAD, DOWNLOAD, or EXIT.");
        }
    }
}

void Client::sendMSG(const std::string& msg) {
    try {
        std::string fullMSG = _userName + " : " + msg;
        boost::asio::write(_socket, boost::asio::buffer(fullMSG + "\n"));
    }
    catch (std::exception& err) {
        printError("Error while sending message : " + std::string(err.what()));
    }
}

void Client::receiveMSG() {
    try {
        char buffer[1024];
        while (_Connected) {
            size_t dataSize = _socket.read_some(boost::asio::buffer(buffer));
            //std::cout<<std::endl;
            std::string serverMessage = std::string(buffer, dataSize);
            std::cout << "\n" << colorText(serverMessage, "36");
            std::cout << colorText("You:", "32") << " ";
            std::cout.flush();
        }
    }
    catch (std::exception& err) {
        printError("Error while sending message : " + std::string(err.what()));
    }
}