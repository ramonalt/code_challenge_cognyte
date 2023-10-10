#pragma once

#include <string>

typedef struct configutarion_struct {
    std::string prefix;
    std::string ip;
    int port;
    size_t max_file_size;
    long long connection_timer;
} configuration;

configuration obtain_configuration_from_env() {
    const std::string prefix = std::getenv("COGNYTE_SERVER_PREFIX");
    const std::string ip = std::getenv("COGNYTE_SERVER_IP");
    const int port = std::stoi(std::getenv("COGNYTE_SERVER_PORT"));
    const size_t max_file_size = std::stol(std::getenv("COGNYTE_SERVER_MAX_FILE_SIZE"));
    const long long connection_timer = std::stol(std::getenv("COGNYTE_SERVER_CONNECTION_TIMER"));

    return configuration{prefix, ip, port, max_file_size, connection_timer};
}