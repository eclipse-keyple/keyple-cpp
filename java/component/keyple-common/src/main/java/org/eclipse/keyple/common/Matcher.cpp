#include "Matcher.h"
#include "Pattern.h"
#include "exceptionhelper.h"
#include "stringhelper.h"

Matcher::Matcher(Pattern* parent, const std::string& text) : parentPattern(parent), text(text)
{
    /* Put fields to initial states */
    reset();
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
    int nextSearchIndex = last;
    if (nextSearchIndex == first)
        nextSearchIndex++;

    /* If next search starts before region, start it at region */
    if (nextSearchIndex < from)
        nextSearchIndex = from;

    /* If next search starts beyond region then it fails */
    if (nextSearchIndex > to) {
        //for (int i = 0; i < (int)groups.size(); i++)
        //    groups[i] = -1;
        return false;
    }

    return search(nextSearchIndex);
}

bool Matcher::find(int start)
{
    int limit = getTextLength();

    if (start < 0 || start > limit)
        throw IndexOutOfBoundException("Illegal start index");

    reset();

    return search(start);
}

    
std::string Matcher::group(int group)
{
    if (group < 0 || group > (int)groups.size())
        throw IndexOutOfBoundException(StringHelper::formatSimple("No group %d", group));

    return groups[group];
}

std::string Matcher::group()
{
    return group(0);
}

bool Matcher::search(int from)
{
    subs = text.substr(from, text.length() - from);
    std::regex word_regex(parentPattern->pattern);

    if (std::regex_search(subs, groups, word_regex))
        return true;

    return false;
}

int Matcher::getTextLength()
{
    return text.length();
}

Matcher *Matcher::reset()
{
    first = -1;
    last = 0;
    oldLast = -1;

    for (int i = 0; i < (int)groups.size(); i++)
        ; //groups[i] = -1;
    for (int i = 0; i < (int)locals.size(); i++)
        locals[i] = -1;

    lastAppendPosition = 0;
    from = 0;
    to = getTextLength();

    return this;
}
