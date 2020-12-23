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

#pragma once

#include <regex>
#include <string>
#include <vector>

#include "KeypleCommonExport.h"

namespace keyple {
namespace common {

class Pattern;

class KEYPLECOMMON_API Matcher {
public:
    /**
     * All matchers have the state used by Pattern during a match.
     */
    Matcher(const Pattern* parent, const std::string& text);

    /**
     * Initiates a search for an anchored match to a Pattern within the given bounds. The groups are
     * filled with default values and the match of the root of the state machine is called. The
     * state machine will hold the state of the match as it proceeds in this matcher.
     */
    bool match(const int from, const int anchor) const;

    /**
     * Attempts to match the entire region against the pattern.
     *
     * <p> If the match succeeds then more information can be obtained via the <tt>start</tt>,
     * <tt>end</tt>, and <tt>group</tt> methods.  </p>
     *
     * @return <tt>true</tt> if, and only if, the entire region sequence matches this matcher's
     *         pattern
     */
    bool matches();

    /**
     * Replaces every subsequence of the input sequence that matches the pattern with the given
     * replacement string.
     *
     * <p> This method first resets this matcher.  It then scans the input sequence looking for
     * matches of the pattern.  Characters that are not part of any match are appended directly to
     * the result string; each match is replaced in the result by the replacement string. The
     * replacement string may contain references to captured subsequences as in the
     * appendReplacement() method.
     *
     * <p> Note that backslashes (<tt>\\</tt>) and dollar signs (<tt>$</tt>) in the replacement
     * string may cause the results to be different than if it were being treated as a literal
     * replacement string. Dollar signs may be treated as references to captured subsequences as
     * described above, and backslashes are used to escape literal characters in the replacement
     * string.
     *
     * <p> Given the regular expression <tt>a*b</tt>, the input <tt>"aabfooaabfooabfoob"</tt>, and
     * the replacement string <tt>"-"</tt>, an invocation of this method on a matcher for that
     * expression would yield the string <tt>"-foo-foo-foo-"</tt>.
     *
     * <p> Invoking this method changes this matcher's state.  If the matcher is to be used in
     * further matching operations then it should first be reset.  </p>
     *
     * @param  replacement The replacement string
     * @return  The string constructed by replacing each matching subsequence by the replacement
     *          string, substituting captured subsequences as needed
     */
    std::string replaceAll(const std::string& replacement) const;

    /**
     * Attempts to find the next subsequence of the input sequence that matches the pattern.
     *
     * This method starts at the beginning of this matcher's region, o, if a previous invocation of
     * the method was successful and the matcher has not since been reset, at the first character
     * not matched by the previous match.
     *
     * If the match succeeds then more information can be obtained viw the start, end, group
     * methods.
     *
     * @return true if, and only if, a subsequence of the input sequences matches this matcher's
     *         patern.
     */
    bool find();

    /**
     * Resets this matcher and then attempts to find the next subsequence of the input sequence that
     * matches the pattern, starting at the specified index.
     *
     * If the match succeeds then more information can be obtained viw the start, end, group
     * methods, and subsequent invocations of the find() method will start at the first character
     * not matched by this match.
     *
     * @return true if, and only if, a subsequence of the input sequences matches this matcher's
     *         patern.
     * @throw IndexOutOfBoundException if start is less than zero of if start is greated than the
     *        length of the input sequence.
     */
    bool find(const int start);

    /**
     * Returns the text that matched a given group of the regular expression. Explicit capturing
     * groups in the pattern are numbered left to right in order of their opening parenthesis,
     * starting at 1. The special group 0 represents the entire match (as if the entier pattern is
     * surrounded by an implicit capturing group).
     *
     * For example, "a((b)c)" matching "abc" would give the following groups:
     * 0 "abc"
     * 1 "bc"
     * 2 "b"
     *
     * An optional capturing group that failed to match as part of an overall sucessful match (for
     * example "a(b)?c" matching "ac") returns null. A capturing group that matched the empty string
     * (for example, "a(b?)c" matching "ac") returns the empty string.
     *
     * @throw IllegalStateException if no successful match has been made
     */
    const std::string group(const int group) const;

    /**
     * Returns the text that matched the whole regular expression.
     *
     * @return the text
     *
     * @throw IllegalStateException if no successful match has been made
     */
    const std::string group() const;

    /**
     * Initiates a search to find a Pattern within the given bounds. The groups are filled with
     * defaults values and the match of the root of the state machine is called. The state machine
     * will hold the state of the match as it proceeds in this matcher.
     *
     * Matcher.from is not set here, because it is the 'hard' boundary of the start of the search
     * which anchors will set to. The from param is the 'soft' boundary of the start of the search,
     * meaning that the regex tries to match at that index but won't match there. Subsequent calls
     * to the search methods start at a new 'soft' boundary which is the end of the previous match.
     */
    bool search(const int from);

    /**
     * Returns the end index of the text.
     *
     * @return the index after the last charaecter in the text
     */
    int getTextLength() const;

    /**
     * Resets this matcher.
     *
     * <p> Resetting a matcher discards all of its explicit states information and sets its append
     * position to zero. The matcher's region is set to the default region, which is its entire
     * character sequence. The anchoring and transparency of this matcher's region boundaries are
     * unaffected.
     */
    Matcher* reset();

private:
    /**
     * The Pattern object that created this Matcher.
     */
    const Pattern* mParentPattern;

    /**
     * The original string being matched.
     */
    const std::string mText;

    /**
     * The range within the sequence that is to be matched. Anchors will match at these "hard"
     * boundaries. Changing the region changes these values.
     */
    int mFrom;
    int mTo;

    /**
     * Matcher state used by the last node. NOANCHOR is used when a match does not have to consume
     * all of the input. ENDANCHOR is the mode used for matching all the input.
     */
    int ENDANCHOR = 1;

    /**
     * The range of string that last matched the pattern. If the last match failed then first is -1;
     * last initially holds 0 then it holds the index of the end of the last match (which is where
     * the next search starts).
     */
    int mFirst = -1, mLast = 0;

    /**
     * The end index of what matched in the last match operation.
     */
    int mOldLast = -1;

    /**
     * Substring on which the regex_search is applied. It used to be a temp variable whithin the
     * Matcher::search() function but for some reasons, on Windows, the 'groups' std::smatch doesn't
     * keep its data after leaving the Matcher::search() function (e.g. in other words, it's very
     * probable that std::match only provides references to specific locations in an existing
     * string).
     */
    std::string mSubs;

    /**
     * The storage used by groups. They may contain invalid values if a group was skipped during the
     * matching.
     */
    std::smatch mGroups;

    /**
     * The index of the last position appended in the substitution.
     */
    int mLastAppendPosition = 0;

    /**
     * Storage used by nodes to tell what repetition they are on in a pattern, and where groups
     * begin. The nodes themselves are stateless, so they rely on this field to hold state during a
     * match.
     */
    std::vector<int> mLocals;
};

}
}
