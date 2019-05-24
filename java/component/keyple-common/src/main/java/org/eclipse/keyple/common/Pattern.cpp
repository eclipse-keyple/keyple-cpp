#include "Pattern.h"

Pattern::Pattern(std::string pattern, int flags) : pattern(pattern), flags(flags)
{

}

Pattern* Pattern::compile(std::string regularExpression, int flags)
{
	return new Pattern(regularExpression, flags);
}

Pattern* Pattern::compile(std::string pattern)
{
	return new Pattern(pattern, 0);
}

Matcher* Pattern::matcher(std::string input)
{
	return new Matcher(this, input);
}
