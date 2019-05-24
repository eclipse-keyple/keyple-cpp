#pragma once

#include <string>

#include "Export.h"
#include "Matcher.h"

class EXPORT Pattern {
public:
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
	Pattern(std::string pattern, int flags);

	/*
	 * Returns a compiled form of the given regular expression, as modified by the
	 * given flags.
	 * 
	 * @throws PatternSyntexException
	 */
	Pattern* compile(std::string regularExpression, int flags);

	/*
	 * Equivalent to Pattern.compile(pattern, 0)
	 */
	static Pattern* compile(std::string pattern);

	/*
	 * Returns a Matcher for this pattern applied to the given input. The Matcher can
	 * be used to match the Pattern against the whole input, find occurences of the
	 * Pattern in the input, or replace parts of the input.
	 */
	Matcher *matcher(std::string input);
};
