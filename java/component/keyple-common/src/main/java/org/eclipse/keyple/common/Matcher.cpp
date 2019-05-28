#include "Matcher.h"
#include "Pattern.h"

Matcher::Matcher(Pattern* parent, const std::string& text) : parentPattern(parent), text(text)
{

}

bool Matcher::match(int from, int anchor)
{
    (void)from;
    (void)anchor;

    /* Alex: let's stop here the Java code copy and try to use C++ regex */
    std::regex word_regex(parentPattern->pattern);
    if (std::regex_search(text, word_regex))
        return true;

    return false;
}

bool Matcher::matches()
{
    return match(from, ENDANCHOR);
}

std::string Matcher::replaceAll(std::string replacement)
{
    (void)replacement;

    /* To be implemented */
    return text;
}

bool Matcher::find()
{
    /* To be implemented */
    return false;
}

bool Matcher::find(int start)
{
    if (start < 0 || start > (int)text.size())
        throw std::runtime_error("Out of bound exception");

    /* To be implemented */
    return false;
}

    
std::string Matcher::group(int group)
{
    (void)group;
        
    /* To be implemented */
    return "";
}

std::string Matcher::group()
{
    return group(0);
}
