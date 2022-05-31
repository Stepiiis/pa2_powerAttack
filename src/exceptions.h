#pragma once
#include <string>
#include <exception>

// logical exception, should never occur
class logException : public std::exception {
public:
    explicit logException(std::string  msg) : _msg(std::move(msg)) {}
    const char* what() const noexcept override {
        return _msg.c_str();
    }
    const std::string _msg;

};

class mapException : public std::exception {
public:
    explicit mapException(std::string  msg) : _msg(std::move(msg)) {}
    const char* what() const noexcept override {
        return _msg.c_str();
    }
    const std::string _msg;
};

class notImplementedException : public std::exception {
public:
    explicit notImplementedException(std::string msg) : _msg(std::move(msg)) {}
    const char* what() const noexcept override {
        return _msg.c_str();
    }
    const std::string _msg;

};

class syntaxErr : public std::exception {
public:
    explicit syntaxErr(std::string  msg) : _msg(std::move(msg)) {}
    const char* what() const noexcept override {
        return _msg.c_str();
    }
    const std::string _msg;
};