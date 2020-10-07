/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include <stdio.h>
#include <sstream>

#include "IndexOutOfBoundException.h"
#include "Matcher.h"
#include "Pattern.h"
#include "stringhelper.h"

namespace keyple {
namespace common {

Matcher::Matcher(Pattern* parent, const std::string& text)
: parentPattern(parent), text(text)
{
    /* Put fields to initial states */
    reset();
}

bool Matcher::match(int from, int anchor)
{
    (void)from;
    (void)anchor;

    if (std::regex_search(text, parentPattern->pattern))
        return true;

    return false;
}

bool Matcher::matches()
{
    return match(mFrom, ENDANCHOR);
}

std::string Matcher::replaceAll(std::string replacement)
{
   std::stringstream ss;

   ss << std::regex_replace(text, parentPattern->pattern, replacement);

   return ss.str();
}

bool Matcher::find()
{
    int nextSearchIndex = last;
    if (nextSearchIndex == first)
        nextSearchIndex++;

    /* If next search starts before region, start it at region */
    if (nextSearchIndex < mFrom)
        nextSearchIndex = mFrom;

    /* If next search starts beyond region then it fails */
    if (nextSearchIndex > mTo) {
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
        throw IndexOutOfBoundException("No group" + std::to_string(group));

    return groups[group];
}

std::string Matcher::group()
{
    return group(0);
}

bool Matcher::search(int from)
{
    subs = text.substr(from, text.length() - from);

    if (std::regex_search(subs, groups, parentPattern->pattern))
        return true;

    return false;
}

int Matcher::getTextLength()
{
    return text.length();
}

Matcher* Matcher::reset()
{
    first   = -1;
    last    = 0;
    oldLast = -1;

    for (int i = 0; i < (int)groups.size(); i++)
        ; //groups[i] = -1;
    for (int i = 0; i < (int)locals.size(); i++)
        locals[i] = -1;

    lastAppendPosition = 0;
    mFrom = 0;
    mTo = getTextLength();

    return this;
}

}
}
