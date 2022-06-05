#include "exceptions.h"

logException::logException(std::string  msg) : _msg(std::move(msg)) {}

const char* logException::what() const noexcept{
    return _msg.c_str();
}

syntaxErr::syntaxErr(std::string msg)  : _msg(std::move(msg)) {}

const char *syntaxErr::what() const noexcept {
    return _msg.c_str();
}

notImplementedException::notImplementedException(std::string msg) : _msg(std::move(msg)) {}

const char *notImplementedException::what() const noexcept {
    return _msg.c_str();
}