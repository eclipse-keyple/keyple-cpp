#pragma once

#include <iostream>
#include <stdexcept>

class Logger {
private:
	bool traceEnabled;
	bool debugEnabled;

	void printf(const char* s)
		{
			while (s && *s) {
			 	if (*s=='%' && *++s!='%')	// make sure that there wasn't meant to be more arguments
								// %% represents plain % in a format string
			         throw std::runtime_error("invalid format: missing arguments");
				std::cout << *s++;
			}
	}

public:
	Logger() {
		traceEnabled = 0;
		debugEnabled = 0;
	}

	bool isTraceEnabled() {
		return traceEnabled;
	}

	bool isDebugEnabled() {
		return debugEnabled;
	}

	template<typename T, typename... Args>
	void trace(const char* s, T value, Args... args);

	template<typename T, typename... Args>
	void debug(const char* s, T value, Args... args);

	template<typename T, typename... Args>
	void debug(T value, Args... args);

	template<typename T, typename... Args>
	void warn(const char *s, T value, Args... args);
};
