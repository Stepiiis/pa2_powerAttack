#pragma once
#include <string>
#include <utility>

class logException : public std::exception {
public:
    explicit logException(std::string  msg) : _msg(std::move(msg)) {}
    const char* what() const noexcept override {
        return _msg.c_str();
    }
    const std::string _msg;

};

bool sendToLogFile(short level, const std::string& text, const std::string& context);


