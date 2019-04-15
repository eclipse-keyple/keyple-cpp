#pragma once

#include <stdexcept>

class Pattern;

class Matcher {
private:
    /*
     * Holds the pattern, that is, the compiled regular expression
     */
    Pattern *pattern;

    /*
     * Holds the input text
     */
    std::string& input;

public:
    /*
     * Constructor
     * 
     * Creates a Matcher for a given combination of pattern and input. Both
     * elements can be changed later on.
     * 
     * @param pattern the pattern to use
     * @param input the input to use
     */
    Matcher(Pattern* pattern, std::string input) : pattern(pattern), input(input)
    {

    }

    bool matches()
        {
        /* To be implemented */
        return false;
    }

    std::string replaceAll(std::string replacement)
    {
        /* To be implemented */
        return replacement;
    }

    /*
     * Attempts to find the next subsequence of the input sequence that matches the pattern.
     * 
     * This method starts at the beginning of this matcher's region, o, if a previous invocation of
     * the method was successful and the matcher has not since been reset, at the first character
     * not matched by the previous match.
     * 
     * If the match succeeds then more information can be obtained viw the start, end, group methods.
     * 
     * @return true if, and only if, a subsequence of the input sequences matches this matcher's patern.
     */
    bool find()
    {
        /* To be implemented */
        return false;
    }

    /*
     * Resets this matcher and then attempts to find the next subsequence of the input sequence that
     * matches the pattern, starting at the specified index.
     * 
     * If the match succeeds then more information can be obtained viw the start, end, group methods,
     * and subsequent invocations of the find() method will start at the first character not matched by
     * this match.
     * 
     * @return true if, and only if, a subsequence of the input sequences matches this matcher's patern.
     * 
     * @throws IndexOutOfBoundException if start is less than zero of if start is greated than the length
     *                                  of the input sequence.
     */
    bool find(int start)
    {
        if (start < 0 || start > (int)input.size())
            throw std::runtime_error("Out of bound exception");

        /* To be implemented */
        return false;
    }

    /*
     * Returns the text that matched a given group of the regular expression. Explicit capturing groups
     * in the pattern are numbered left to right in order of their opening parenthesis, starting at 1. The
     * special group 0 represents the entire match (as if the entier pattern is surrounded by an implicit
     * capturing group).
     * 
     * For example, "a((b)c)" matching "abc" would give the following groups:
     * 0 "abc"
     * 1 "bc"
     * 2 "b"
     * 
     * An optional capturing group that failed to match as part of an overall sucessful match (for
     * example "a(b)?c" matching "ac") returns null. A capturing group that matched the empty string (for
     * example, "a(b?)c" matching "ac") returns the empty string.
     * 
     * @throws IllegalStateException if no successful match has been made
     */
    std::string group(int group)
    {
        (void)group;
        
        /* To be implemented */
        return "";
    }

    /*
     * Returns the text that matched the whole regular expression.
     * 
     * @return the text
     * 
     * @throws IllegalStateException if no successful match has been made
     */
    std::string group()
    {
        return group(0);
    }
};