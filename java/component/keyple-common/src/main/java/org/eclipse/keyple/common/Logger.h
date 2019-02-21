#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

class Logger {

  private:
    /**
     *
     */
    bool traceEnabled;

    /**
     *
     */
    bool debugEnabled;

    /**
     *
     */
    //const std::string className;

    void log(const char *s)
    {
        while (s && *s)
        {
            if (*s == '%' && *++s != '%') // make sure that there wasn't meant to be more arguments
                                          // %% represents plain % in a format string
                throw std::runtime_error("invalid format: missing arguments");
            std::cout << *s++;
        }
    }

  public:
    /**
	 * Constructor
	 */
    Logger(const std::string &className)// : className(className)
    {
        std::cout << "[Logger::Logger] creating logger for class " << std::endl;

        traceEnabled = 0;
        debugEnabled = 0;
    }

    /**
     * Destructor
     */
    ~Logger()
    {
        std::cout << "[Logger::~Logger] destroying logger" << std::endl;
    }

    bool isTraceEnabled()
    {
        return traceEnabled;
    }

    bool isDebugEnabled()
    {
        return debugEnabled;
    }

    void trace(const char *s)
    {
        log(s);
    }

    template <typename T, typename... Args> void trace(const char *s, T value, Args... args)
    {
        while (s && *s)
        {
            if (*s == '%' && *++s != '%')
            {                               // a format specifier (ignore which one it is)
                std::cout << value;         // use first non-format argument
                return trace(++s, args...); // ``peel off'' first argument
            }
            std::cout << *s++;
        }
        throw std::runtime_error("extra arguments provided to printf");
    }

    void debug(const char *s)
    {
        log(s);
    }

    template <typename T, typename... Args> void debug(const char *s, T value, Args... args)
    {
        while (s && *s)
        {
            if (*s == '%' && *++s != '%')
            {                               // a format specifier (ignore which one it is)
                std::cout << value;         // use first non-format argument
                return debug(++s, args...); // ``peel off'' first argument
            }
            std::cout << *s++;
        }
        throw std::runtime_error("extra arguments provided to printf");
    }

    void warn(const char *s)
    {
        log(s);
    }

    template <typename T, typename... Args> void warn(const char *s, T value, Args... args)
    {
        while (s && *s)
        {
            if (*s == '%' && *++s != '%')
            {                              // a format specifier (ignore which one it is)
                std::cout << value;        // use first non-format argument
                return warn(++s, args...); // ``peel off'' first argument
            }
            std::cout << *s++;
        }
        throw std::runtime_error("extra arguments provided to printf");
    }

    void info(const char *s)
    {
        log(s);
    }

    template <typename T, typename... Args> void info(const char *s, T value, Args... args)
    {
        while (s && *s)
        {
            if (*s == '%' && *++s != '%')
            {                              // a format specifier (ignore which one it is)
                std::cout << value;        // use first non-format argument
                return info(++s, args...); // ``peel off'' first argument
            }
            std::cout << *s++;
        }
        throw std::runtime_error("extra arguments provided to printf");
    }
};
