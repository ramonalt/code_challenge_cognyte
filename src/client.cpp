#include <thread>
#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

#include "configuration.hpp"

using namespace boost::asio;
using ip::tcp;


void interactive_mode(const configuration& config) {
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);
    
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(config.ip), config.port));
    boost::system::error_code error; 

    do {
        std::string input_data;

        std::cout << "Input data: ";
        std::cin >> input_data;
        std::cout << "Data going to be sent: " << input_data << '\n';
        
        boost::asio::write(socket, boost::asio::buffer(input_data), error);
    } while(!error);

    std::cout << "Exiting...";

}




int main(int argc, char* argv[]) {    
    const configuration config = obtain_configuration_from_env();
    interactive_mode(config);

    return 0;
}