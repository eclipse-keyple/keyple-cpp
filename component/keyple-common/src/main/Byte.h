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

#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Integer.h"
#include "Object.h"

namespace keyple {
namespace common {

class Byte : public Object {
private:
    /*
     * The value of the Byte
     */
    unsigned char value;

    /*
     *
     */
    class ByteCache {
    public:
        static std::vector<Byte *> cache;

        /*
         *
         */
        ByteCache()
        {
            for (int i = 0; i < 256; i++)
                cache.push_back(new Byte((unsigned char)(i - 128)));
        }

    };

    /*
     *
     */
    static ByteCache byteCache;

public:
    /*
     * 
     *
     *
     */
    Byte()
    {
        this->value = 0;
    }

    /*
     * Constructor
     * 
     * Constructs a newly allocated Byte object that represents the specified
     * byte value.
     * 
     * @param value the value represented by the Byte
     */
    Byte(unsigned char value)
    {
        this->value = value;
    }

    /**
     *
     */
    Byte(const Byte& b) : Object()
    {
        this->value = b.value;
    }

    /**
     *
     */
    virtual ~Byte() {}

    /**
     *
     */
    Byte& operator=(Byte other)
    {
        this->value = other.value;

        return *this;
    }

    /*
     * Returns a Byte instance representing the specified value.
     * 
     * If a new Byte instance is not required this method should generally be
     * used in preference to the constructor Byte(unsigned char), as this
     * method is likely to yield significantly better space and time
     * performance since all byte values are cached.
     */
    static Byte* valueOf(unsigned char b)
    {
        int offset = 128;

        return ByteCache::cache[(int)b + offset];
    }

    /*
     * Returns a new string object representing the specified byte. The radix
     * is assumed to be 10.
     * 
     * @param b the byte to be converted
     * 
     * @return the string representation of the specified byte
     */
    static std::string toString(unsigned char b)
    {
        return Integer::toString(b);
    }

    /*
     * Returns a string object representing this Byte's value. The value is
     * converted to signed decimal representation and returned as a string,
     * exactly as if the integer value were given as an argument to the
     * toString method.
     * 
     * @return a string representation of the value of this object in base 10
     */
    std::string toString()
    {
        return Integer::toString((int)value);
    }

    /*
     * Returns a hash code for this Byte, equal to the result of invoking
     * intValue().
     * 
     * @return a hash code value for this Byte
     */
    int hashCode() override
    {
        return Byte::hashCode(value);
    }

    /*
     * Returns a hash code for a bytes value; compatible with Byte::hashCode().
     * 
     * @param value the value to hash
     * 
     * @return a hash code value for a byte value
     */
    static int hashCode(unsigned char value)
    {
        return (int)value;
    }

    /*
     * Compares this object to the specified object. The result is true if and
     * only if the argument is not null and is a Byte object that contains the
     * same byte value as this object.
     * 
     * @param obj the object to compare with
     * 
     * @return true if the objects are the same, false otherwise
     */
    bool equals(Object* obj)
    {
        if (dynamic_cast<Byte*>(obj))
            return value == dynamic_cast<Byte*>(obj)->byteValue();

        return false;
    }

    /*
     * Alex: I would rather have a pointer here rather than a smart pointer
     *       (as above) but I will keep this one for now as well.
     */
    bool equals(std::shared_ptr<void> o) override
    {
        if (std::static_pointer_cast<Byte>(o))
            return value == std::static_pointer_cast<Byte>(o)->byteValue();

        return false;
    }

    /*
     * Returns the value of this Byte as a byte.
     */
    unsigned char byteValue()
    {
        return value;
    }

    /*
     *
     */
    void finalize() override
    {
    }

    /*
     *
     */
    bool operator==(const Byte& other)
    {
        return this->value == other.value;
    }
};

}
}

