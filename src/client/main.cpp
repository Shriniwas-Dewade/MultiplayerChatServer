#include "client.hpp"

int main() {
    std::string addr;
    unsigned short port;
    std::string name;

    std::cout << "\n" << colorText("========================================", "34") << std::endl;
    std::cout << colorText("  Welcome to the Multiplayer Chat Client", "33") << std::endl;
    std::cout << colorText("========================================", "34") << "\n";

     std::cout << colorText("Enter Server IP:", "36") << " ";
    std::cin >> addr;
    std::cout << colorText("Enter Server Port:", "36") << " ";
    std::cin >> port;

    std::cin.ignore();
    std::cout << colorText("Enter Your Name:", "36") << " ";
    std::getline(std::cin, name);

    Client client(addr, port, name);
    client.start();
}