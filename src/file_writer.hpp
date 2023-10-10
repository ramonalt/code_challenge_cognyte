#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "configuration.hpp"

inline const std::string get_date() {
    using namespace boost::posix_time;

    ptime now = second_clock::universal_time();
    static std::locale loc(std::cout.getloc(), new time_facet("_%Y%m%d%H%M%S"));

    std::basic_stringstream<char> ss;
    ss.imbue(loc);
    ss << now;
    return ss.str();
}

inline const std::string get_filename(const configuration& config) {
    std::string filename = config.prefix + get_date();
    return filename;
}

class FileWriter {
    private:
        size_t max_size;
        size_t bytes_written;
        size_t connection_counter;
        size_t counter = 0;
        std::string filename;
        std::ofstream ofs;

        void open_new_file() {
            this->counter += 1;
            this->bytes_written = 0;

            std::string filepath = "files/" + filename + "_" + std::to_string(this->connection_counter) + "_" + std::to_string(this->counter);
            this->ofs.open(
                filepath,
                std::ios::out | std::ios::app | std::ios::ate
            );
        }
    
    
    public:
        FileWriter(const size_t connection_counter, const configuration& config) : connection_counter(connection_counter) {
            this->max_size = config.max_file_size;
            this->filename = get_filename(config);
            
            this->open_new_file();
        };

        ~FileWriter() {
            this->ofs.close();
        }

        size_t write(boost::asio::streambuf& buffer) { 
            size_t available_space = this->max_size - this->bytes_written;
            size_t count = std::min(available_space, buffer.size());
            
            this->ofs.write(boost::asio::buffer_cast<const char*>(buffer.data()), count);
            this->bytes_written += count;

            if (this->is_full()) {
                this->ofs.close();
                this->open_new_file();
            }

            return count;
        };

        bool is_full() const {
            return this->bytes_written == this->max_size;
        }
};