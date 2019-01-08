#pragma once

#include <exception>
#include <stdexcept>

class Exception : public std::exception
{
public:
	Exception(const std::string &message) {

	}

	Exception(const std::string &message, std::runtime_error cause) {

	}

	void printStackTrace() const {

	}
};
