#pragma once

class Object {

	virtual bool equals(std::shared_ptr<void> o) = 0;

	virtual int hashCode() = 0;
};
