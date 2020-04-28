#pragma once

#include <string>

/* Common */
#include "Character.h"
#include "exceptionhelper.h"
#include "stringhelper.h"

namespace keyple {
namespace common {

class Integer {
private:
    /*
     * The value of the Integer
     */
    int value;

    /**
     * The minimum value an <code>int</code> can represent is -2147483648 (or
     * -2<sup>31</sup>).
     */
    static const int MIN_VALUE = 0x80000000;

    /**
     * The maximum value an <code>int</code> can represent is 2147483647 (or
     * 2<sup>31</sup> - 1).
     */
    static const int MAX_VALUE = 0x7fffffff;

public:
    /*
     * Constructor
     * 
     * Constructs a newly allocated Integer object that represents the
     * specified int value.
     * 
     * @param value the value to be represeneed by the Integer object
     */
    Integer(int value)
    {
        this->value = value;
    }

    /*
     * Returns a string representation of the first aguments in the
     * radix specified by the second argument.
     */
    static std::string toString(int i, int radix)
    {
        if (radix == 10)
            return StringHelper::to_string(i);
        else
            return "radix != 10 not handled yet";
    }

    /*
     * Returns a string object representing the specified integer. The argument
     * is converted to signed decimal representation and returned as a string,
     * exactly as if the argument and radix 10 were given as arguments to the
     * method.
     * 
     * @param i an integer to be converted
     * 
     * @return a string representation of the argument in base 10
     */
    static std::string toString(int i)
    {
        return toString(i, 10);
    }

    /*
     * Returns a string object representing this Integer's value. The value is
     * converted to signed decimal representation and returned as a string,
     * exactly as if the integer value were given as an argument to the
     * toString method.
     * 
     * @return a string representation of the value of this object in base 10
     */
    std::string toString()
    {
        return toString(value);
    }

    /**
     * Converts the specified <code>String</code> into an <code>int</code>
     * using the specified radix (base). The string must not be <code>null</code>
     * or empty. It may begin with an optional '-', which will negate the answer,
     * provided that there are also valid digits. Each digit is parsed as if by
     * <code>Character.digit(d, radix)</code>, and must be in the range
     * <code>0</code> to <code>radix - 1</code>. Finally, the result must be
     * within <code>MIN_VALUE</code> to <code>MAX_VALUE</code>, inclusive.
     * Unlike Double.parseDouble, you may not have a leading '+'.
     *
     * @param str the <code>String</code> to convert
     * @param radix the radix (base) to use in the conversion
     * @return the <code>String</code> argument converted to <code>int</code>
     * @throws NumberFormatException if <code>s</code> cannot be parsed as an
     *         <code>int</code>
     */
    static int parseInt(std::string str, int radix)
    {
        return parseInt(str, radix, false);
    }

    /**
     * Helper for parsing ints, used by Integer, Short, and Byte.
     *
     * @param str the string to parse
     * @param radix the radix to use, must be 10 if decode is true
     * @param decode if called from decode
     * @return the parsed int value
     * @throws NumberFormatException if there is an error
     * @throws NullPointerException if decode is true and str if null
     * @see #parseInt(String, int)
     * @see #decode(String)
     * @see Byte#parseByte(String, int)
     * @see Short#parseShort(String, int)
     */
    static int parseInt(std::string str, int radix, bool decode)
    {
        if (!decode && str.empty())
            throw new NumberFormatException();

        int index  = 0;
        int len    = str.length();
        bool isNeg = false;

        if (len == 0)
            throw new NumberFormatException("string length is null");

        int ch = str.at(index);
        if (ch == '-') {
            if (len == 1)
                throw new NumberFormatException("pure '-'");
            isNeg = true;
            ch    = str.at(++index);
        }

        if (decode) {
            if (ch == '0') {
                if (++index == len)
                    return 0;
                if ((str.at(index) & ~('x' ^ 'X')) == 'X') {
                    radix = 16;
                    index++;
                } else
                    radix = 8;
            } else if (ch == '#') {
                radix = 16;
                index++;
            }
        }

        if (index == len)
            throw new NumberFormatException("non terminated number: " + str);

        int max = MAX_VALUE / radix;
        // We can't directly write `max = (MAX_VALUE + 1) / radix'.
        // So instead we fake it.
        if (isNeg && MAX_VALUE % radix == radix - 1)
            ++max;

        int val = 0;
        while (index < len) {
            if (val < 0 || val > max)
                throw NumberFormatException(StringHelper::formatSimple(
                    "number overflow (pos= %d) : %s", index, str));

            ch  = Character::digit(str.at(index++), radix);
            val = val * radix + ch;
            if (ch < 0 || (val < 0 && (!isNeg || val != MIN_VALUE)))
                throw NumberFormatException(StringHelper::formatSimple(
                    "invalid character at position %d in %s", index, str));
        }

        return isNeg ? -val : val;
    }
};

}
}
