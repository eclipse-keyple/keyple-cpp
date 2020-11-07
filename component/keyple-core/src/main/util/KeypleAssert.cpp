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

#include "KeypleAssert.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace core {
namespace util {

using namespace keyple::common::exception;

const KeypleAssert KeypleAssert::INSTANCE;

KeypleAssert::KeypleAssert() {}

const KeypleAssert& KeypleAssert::getInstance()
{
    return INSTANCE;
}

const KeypleAssert& KeypleAssert::notNull(void* obj, const std::string& name)
    const
{
    if (obj == nullptr)
        throw IllegalArgumentException("Argument [" + name + "] is null.");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::notEmpty(const std::string& obj,
                                           const std::string& name) const
{
    if (obj.empty())
        throw IllegalArgumentException("Argument [" + name + "] is empty.");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::notEmpty(const std::vector<uint8_t> obj,
                                           const std::string& name) const
{
    if (obj.empty())
        throw IllegalArgumentException("Argument [" + name + "] is empty.");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::isTrue(const bool condition,
                                         const std::string& name) const
{
    if (!condition)
        throw IllegalArgumentException("Condition [" + name + "] is false.");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::greaterOrEqual(const int number,
                                                 const int minValue,
                                                 const std::string& name) const
{
    if (number < minValue)
        throw IllegalArgumentException("Argument [" + name + "] " +
                                       "has a value [" + std::to_string(number) + "] " +
                                       "less than [" + std::to_string(minValue) + "].");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::isEqual(const int number,
                                          const int value,
                                          const std::string& name) const
{
    if (number != value)
        throw IllegalArgumentException("Argument [" + name + "] " +
                                       "has a value [" + std::to_string(number) + "] " +
                                       "not equal to [" + std::to_string(value) + "].");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::isInRange(const int number,
                                            const int minValue,
                                            const int maxValue,
                                            const std::string& name) const
{
    if (number < minValue)
        throw IllegalArgumentException("Argument [" + name + "] " +
                                       "has a value [" + std::to_string(number) + "] " +
                                       "less than [" + std::to_string(minValue) + "].");

    else if (number > maxValue)
        throw IllegalArgumentException("Argument [" + name + "] " +
                                       "has a value [" + std::to_string(number) + "] " +
                                       "more than [" + std::to_string(maxValue) + "].");

    return INSTANCE;
}

}
}
}
