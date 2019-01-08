#include "Logger.h"

template<typename T, typename... Args>
void trace(const char* s, T value, Args... args) {
	while (s && *s) {
		if (*s=='%' && *++s!='%') {	// a format specifier (ignore which one it is)
			std::cout << value;		// use first non-format argument
			return trace(++s, args...); 	// ``peel off'' first argument
		}
		std::cout << *s++;
	}
	throw std::runtime_error("extra arguments provided to printf");
}

template<typename T, typename... Args>
void debug(const char* s, T value, Args... args) {
	while (s && *s) {
		if (*s=='%' && *++s!='%') {	// a format specifier (ignore which one it is)
			std::cout << value;		// use first non-format argument
			return debug(++s, args...); 	// ``peel off'' first argument
		}
		std::cout << *s++;
	}
	throw std::runtime_error("extra arguments provided to printf");
}
