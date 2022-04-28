#pragma once
#include <string>

class logException : public std::exception {
public:
    logException(const std::string& msg) : _msg(msg) {}
    const char* what() const throw() {
        return _msg.c_str();
    }
    std::string _msg;

};

bool sendToLogFile(short level, const std::string& text, const std::string context);


