#pragma once
#include <stdexcept>
#include <string>

class UserException : public std::runtime_error {
public:
	explicit UserException(const std::string& msg) : std::runtime_error(msg) {}
};
