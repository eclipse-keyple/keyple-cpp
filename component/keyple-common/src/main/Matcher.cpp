/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "Matcher.h"
#include "Pattern.h"
#include "exceptionhelper.h"
#include "stringhelper.h"

namespace keyple {
namespace common {

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
    std::string init_string = text;
    std::string result;
    bool bDone = true;

    for ( unsigned int index = 0; index < parentPattern->pattern.size(); index++ )
    {
        char sBuf[4];

        snprintf( sBuf, sizeof(sBuf), "%c", parentPattern->pattern[index] );
        std::string find = sBuf;
        size_t find_len = find.size();
        size_t pos,from=0;
        while ( std::string::npos != ( pos=init_string.find(find,from) ) ) {
            result.append( init_string, from, pos-from );
            result.append( replacement );
            from = pos + find_len;
            bDone = true;
        }
        if ( bDone )
        {
            bDone = false;
            result.append( init_string, from , std::string::npos );
        }
        init_string.clear();
        init_string = result;
    }
    return init_string;

//    /* To be implemented */
//    return text;
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

}
}

