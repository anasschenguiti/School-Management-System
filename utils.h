// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stdexcept>

class AuthException : public std::runtime_error {
public:
    AuthException(const std::string& message) : std::runtime_error(message) {}
};

class FileException : public std::runtime_error {
public:
    FileException(const std::string& message) : std::runtime_error(message) {}
};

#endif
