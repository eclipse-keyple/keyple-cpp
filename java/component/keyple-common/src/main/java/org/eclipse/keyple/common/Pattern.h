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

		std::string replaceAll(std::string replacement) {
			/* To be implemented */
			return replacement;
		}
	};

	static Pattern& compile(std::string regex) {
		/* To be implemented */
		static Pattern p;
    	return p;
	}

	static Matcher &matcher(std::string input)
	{
		static Matcher privateMatcher;
		return privateMatcher;
	}
};
