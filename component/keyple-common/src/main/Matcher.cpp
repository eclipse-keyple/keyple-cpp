/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include <stdio.h>
#include <sstream>

#include "IndexOutOfBoundsException.h"
#include "Matcher.h"
#include "Pattern.h"
#include "stringhelper.h"

namespace keyple {
namespace common {

using namespace keyple::common::exception;

Matcher::Matcher(const Pattern* parent, const std::string& text)
: mParentPattern(parent), mText(text)
{
    /* Put fields to initial states */
    reset();
}

bool Matcher::match(const int from, const int anchor) const
{
    (void)from;
    (void)anchor;

    if (std::regex_search(mText, mParentPattern->mPattern))
        return true;

    return false;
}

bool Matcher::matches()
{
    return match(mFrom, ENDANCHOR);
}

std::string Matcher::replaceAll(const std::string& replacement) const
{
   std::stringstream ss;

   ss << std::regex_replace(mText, mParentPattern->mPattern, replacement);

   return ss.str();
}

bool Matcher::find()
{
    int nextSearchIndex = mLast;
    if (nextSearchIndex == mFirst)
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

bool Matcher::find(const int start)
{
    int limit = getTextLength();

    if (start < 0 || start > limit)
        throw IndexOutOfBoundsException("Illegal start index");

    reset();

    return search(start);
}

const std::string Matcher::group(const int group) const
{
    if (group < 0 || group > (int)mGroups.size())
        throw IndexOutOfBoundsException("No group" + std::to_string(group));

    return mGroups[group];
}

const std::string Matcher::group() const
{
    return group(0);
}

bool Matcher::search(const int from)
{
    mSubs = mText.substr(from, mText.length() - from);

    if (std::regex_search(mSubs, mGroups, mParentPattern->mPattern))
        return true;

    return false;
}

int Matcher::getTextLength() const
{
    return mText.length();
}

Matcher* Matcher::reset()
{
    mFirst   = -1;
    mLast    = 0;
    mOldLast = -1;

    for (int i = 0; i < (int)mGroups.size(); i++)
        ; //groups[i] = -1;
    for (int i = 0; i < (int)mLocals.size(); i++)
        mLocals[i] = -1;

    mLastAppendPosition = 0;
    mFrom = 0;
    mTo = getTextLength();

    return this;
}

}
}
