#pragma once

#include <string>

class Boolean {
private:
    /*
     * The value of the Boolean
     */
    bool value;

public:
    /*
     * Allocates a Boolean object representing the arguments.
     * 
     * Note: it is rarely appropriate to use this constructore. Unless a new
     * instance is required, the static factory valueOf(boolean) is generally
     * a better choice. It is likely to yield significantly better spce and
     * time performance.
     * 
     * @param value the value of the Boolean
     */
    Boolean(bool value)
    {
        this->value = value;
    }

    /*
     * Returns a Boolean instance representing the specified Boolean value. If
     * the specified Boolean value is true, this method returns Boolean.TRUE;
     * if it is false, this method returns Boolean.FALSE. If a new Boolean
     * instance is not required, this method should generally be used in
     * preference to the constructor Boolean(bool), as this method is likely to
     * yield significantly better space and time performance.
     * 
     * @param b a boolean value
     *
     * @return a Boolean instance representing b
     */
    static Boolean valueOf(bool b)
    {
        /*
         * The Boolean object corresponding to the primitive value true.
         */
        static Boolean TRUE(true);

        /*
         * The Boolean object corresponding to the primitive value false.
         */
        static Boolean FALSE(false);

        return b ? TRUE : FALSE;
    }

    /*
     * Returns a string object representing the specified boolean. If the
     * specified boolean is true, then the string "true" will be returned,
     * otherwise the string "false" will be returned.
     * 
     * @param b the boolean to be converted
     * 
     * @return the string representation of the sepcified boolean
     */
    static std::string toString(bool b)
    {
        return b ? "true" : "false";
    }

    /*
     * Returns a string object repesenting this Boolean's value. If this
     * object represents the value true, a string equal to "true" is
     * returned. Otherwise, a string equal to "false" is returned.
     * 
     * @return a string representation of this object
     */
    std::string toString()
    {
        return value ? "true" : "false";
    }

};
