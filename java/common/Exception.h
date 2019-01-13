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

	/**
	 * Returns the detail message string of this throwable.
	 *
	 * @return the detail message string of this Throwable instance (which may
	 * be null).
	 */
	std::string getMessage()
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
	std::string getCause()
	{
		return cause;
	}

private:
	/**
	 * Part of Jave Throwable class.
	 */
	std::string message;

	/**
	 *
	 */
	std::string cause;
};
