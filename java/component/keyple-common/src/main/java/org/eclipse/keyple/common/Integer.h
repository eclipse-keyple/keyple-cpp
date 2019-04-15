#pragma once

#include <string>

class Integer {
private:
    /*
     * The value of the Integer
     */
    int value;

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
            return std::to_string(i);
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
};
