#pragma once

#include <stdexcept>

class Object {
public:
	virtual bool equals(std::shared_ptr<void> o) = 0;

	virtual int hashCode() = 0;

protected:
	virtual void finalize() throw(std::runtime_error) = 0;
};
