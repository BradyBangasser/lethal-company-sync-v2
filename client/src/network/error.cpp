#include "network.hpp"
#include <string>
#include <fmt/core.h>

network::NetworkError::NetworkError(int code, std::string msg) {
    this->code = code;
    this->msg = msg;
}

const char *network::NetworkError::what() {
    fmt::print(stderr, "Fix this");
    std::string message = fmt::format("[{}] {}", code, msg);
    return msg.c_str();
}