#include "server.h"
#include <iostream>


int main(int argc, char * argv[]) {
    std::cout << "Hello from bulk_server!" << std::endl;
    try {
        if(argc != 3) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";return 1;

        }
        boost::asio::io_context o_context;
        boost::asio::io_context io_context;
        bulk::Server server(io_context, std::stoi(argv[1]), std::atoi(argv[2]));
        io_context.run();

    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}