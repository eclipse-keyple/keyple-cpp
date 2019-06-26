#pragma once

#include <exception>
#include <stdexcept>
#include <iostream>

class Exception : public std::exception
{
public:
    Exception(const std::string &message) : message(message), cause("no cause")
    {

	}

	Exception(const std::string &message, const std::runtime_error cause) : message(message), cause(cause)
    {

	}

	void printStackTrace() const {

	}

	/**
	 * Returns the detail message string of this throwable.
	 *
	 * @return the detail message string of this Throwable instance (which may
	 * be null).
	 */
	std::string getMessage() const
	{
		return message;
	}

	/**
	 * Returns the cause of this throwable or null if the cause is nonexistent
	 * or unknown. (The cause is the throwable that caused this throwable to get
	 * thrown.)
	 *
	 * This implementation returns the cause that was supplied via one of the
	 * constructors requiring a Throwable, or that was set after creation with
	 * the initCause(Throwable) method. While it is typically unnecessary to
	 * override this method, a subclass can override it to return a cause set by
	 * some other means. This is appropriate for a "legacy chained throwable"
	 * that predates the addition of chained exceptions to Throwable. Note that
	 * it is not necessary to override any of the PrintStackTrace methods, all
	 * of which invoke the getCause method to determine the cause of a
	 * throwable.
	 *
	 * @return the cause of this throwable or null if the cause is nonexistent
	 *         or unknown.
	 */
	const std::runtime_error getCause() const
	{
		return cause;
	}

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream &os, const Exception &e)
    {
        os << "message: " << e.getMessage().c_str() << ", cause: " << e.getCause().what();
        return os;
    }

private:
	/**
	 * Part of Jave Throwable class.
	 */
	const std::string message;

	/**
	 *
	 */
	const std::runtime_error cause;
};
