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

#include <string>
#include <vector>

 /* Core */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace util {

/**
 * The utility class {@code Assert} exposes very useful methods for testing method call parameters
 * and raising a {@link IllegalArgumentException} unchecked exception.
 *
 * @since 0.9
 */
class KEYPLECORE_API KeypleAssert final {
public:
    /**
     * Gets the unique instance.
     *
     * @return the instance
     */
    static const KeypleAssert& getInstance();

    /**
     * Assert that the input object is not null.
     *
     * @param obj the object to check
     * @param name the object name
     * @return the current instance
     * @throw IllegalArgumentException if object is null
     * @since 0.9
     */
    const KeypleAssert& notNull(void* obj, const std::string& name) const;

    /**
     * Assert that the input string is not null and not empty.
     *
     * @param obj the object to check
     * @param name the object name
     * @return the current instance
     * @throw IllegalArgumentException if object is null or empty
     * @since 0.9
     */
    const KeypleAssert& notEmpty(const std::string& obj, const std::string& name) const;

    /**
     * Assert that a list of objects is not null and not empty.
     *
     * @param obj the object to check
     * @param name the object name
     * @return the current instance
     * @throw IllegalArgumentException if object is null or empty
     * @since 0.9
     */
    // const KeypleAssert& notEmpty(List<? extends Object> obj, String name) {
    //     if (obj == null) {
    //         throw new IllegalArgumentException("Argument [" + name + "] is null.");
    //     }
    //     if (obj.isEmpty()) {
    //         throw new IllegalArgumentException("Argument [" + name + "] is empty.");
    //     }
    //     return INSTANCE;
    // }

    /**
     * Assert that a byte array is not null and not empty.
     *
     * @param obj the object to check
     * @param name the object name
     * @return the current instance
     * @throw IllegalArgumentException if object is null or empty
     * @since 0.9
     */
    const KeypleAssert& notEmpty(const std::vector<uint8_t> obj,  const std::string& name) const;

    /**
     * Assert that a condition is true.
     *
     * @param condition the condition to check
     * @param name the object name
     * @return the current instance
     * @throw IllegalArgumentException if condition is null or false
     * @since 0.9
     */
    const KeypleAssert& isTrue(const bool condition, const std::string& name) const;

    /**
     * Assert that an integer is not null and is greater than or equal to minValue.
     *
     * @param number the number to check
     * @param minValue the min accepted value
     * @param name the object name
     * @return the current instance
     * @throw IllegalArgumentException if number is null or has a value less  than minValue.
     * @since 0.9
     */
    const KeypleAssert& greaterOrEqual(const int number,
                                       const int minValue,
                                       const std::string& name) const;

    /**
     * Assert that an integer is equal to value.
     *
     * @param number the number to check
     * @param value the expected value
     * @param name the object name
     * @return the current instance
     * @throw IllegalArgumentException if number is null or has a value less than minValue.
     * @since 0.9
     */
    const KeypleAssert& isEqual(const int number,  const int value, const std::string& name) const;

    /**
     * Assert that an integer is not null and is in the range minValue, maxValue.
     *
     * @param number the number to check
     * @param minValue the min accepted value
     * @param maxValue the max accepted value
     * @param name the object name
     * @return the current instance
     * @throw IllegalArgumentException if number is null or is out of range.
     * @since 0.9
     */
    const KeypleAssert& isInRange(const int number,
                                  const int minValue,
                                  const int maxValue,
                                  const std::string& name) const;

private:
    /**
     * Singleton pattern
     */
    static const KeypleAssert INSTANCE;

    /**
     * Private Constructor
     */
    KeypleAssert();
};

}
}
}
