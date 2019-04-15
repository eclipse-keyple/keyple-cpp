#pragma once

#include <string>

#include "Matcher.h"

class Pattern {
private:
	/*
	 *
	 */
	std::string pattern;

	/*
	 *
	 */
	int flags;

public:
	/*
	 * Constructor
	 */
	Pattern(std::string pattern, int flags) : pattern(pattern), flags(flags)
	{

	}

	/*
	 * Returns a compiled form of the given regular expression, as modified by the
	 * given flags.
	 * 
	 * @throws PatternSyntexException
	 */
	Pattern* compile(std::string regularExpression, int flags)
	{
		return new Pattern(regularExpression, flags);
	}

	/*
	 * Equivalent to Pattern.compile(pattern, 0)
	 */
	static Pattern* compile(std::string pattern)
	{
		return new Pattern(pattern, 0);
	}

	/*
	 * Returns a Matcher for this pattern applied to the given input. The Matcher can
	 * be used to match the Pattern against the whole input, find occurences of the
	 * Pattern in the input, or replace parts of the input.
	 */
	Matcher *matcher(std::string input)
	{
		return new Matcher(this, input);
	}
};
