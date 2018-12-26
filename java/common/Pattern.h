#pragma once

#include <string>

class Pattern {

public:
	class Matcher {
	public:
		bool matches() {
			/* To be implemented */
			return false;
		}
	};

private:
	static Matcher privateMatcher;

public:
	static Pattern &compile(std::string regex) {
		/* To be implemented */
		static Pattern p;
    	return p;
	}

	static Matcher &matcher(std::string input)
	{
		return privateMatcher;
	}
};
