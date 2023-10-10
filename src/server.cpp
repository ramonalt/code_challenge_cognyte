#include <iostream>
#include <string>
#include <utility>
#include <chrono>
#include <thread>
#include <map>
#include <boost/asio.hpp>

#include "configuration.hpp"
#include "file_writer.hpp"
#include "atomic_queue.hpp"

using namespace boost::asio;
using ip::tcp;

void handle_socket(size_t counter, ip::tcp::socket& socket, const configuration& config, atomic_queue<size_t>& joinable_threads)
{
    boost::asio::streambuf buffer(config.max_file_size);
    
    auto now = [] () {return std::chrono::system_clock::now();};
    auto curr_time = now();
    auto duration = [&curr_time, &now] () {return std::chrono::duration_cast<std::chrono::milliseconds>(now() - curr_time);};

    FileWriter writer(counter, config);

    do {
        int bytes_transferred = boost::asio::read(socket, buffer, [&socket] (const auto& error, auto bytes_transferred) {
            return socket.available();
        });

        if (bytes_transferred > 0) {
            curr_time = now();

            size_t bytes_written = writer.write(buffer);
            buffer.consume(bytes_written);
        }
        
    } while(socket.available() > 0 || duration().count() < config.connection_timer);

    socket.close();
    joinable_threads.atomic_push(counter);
}

void handle_connections(const configuration& config) {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor_(io_service, ip::tcp::endpoint(ip::tcp::v4(), config.port));
    
    std::map<size_t, std::thread> threads = {};
    std::map<size_t, tcp::socket*> sockets = {};
    size_t counter = 0;
    
    atomic_queue<size_t> joinable_threads;
    
    while (true) {
        sockets[counter] = new tcp::socket(io_service);
        tcp::socket& socket_ =*sockets[counter];

        acceptor_.accept(socket_);

        threads[counter] = std::thread(handle_socket, counter, std::ref(socket_), config, std::ref(joinable_threads));
        counter++;

        while (joinable_threads.atomic_size() > 0) {
            size_t key = joinable_threads.atomic_front_and_pop();
            
            std::thread& joinable_thread = threads[key];
            joinable_thread.join();

            delete sockets[key];
            sockets.erase(key);
            threads.erase(key);
        }
    }
};

int main()
{
    const configuration config = obtain_configuration_from_env();
    handle_connections(config);
    
}