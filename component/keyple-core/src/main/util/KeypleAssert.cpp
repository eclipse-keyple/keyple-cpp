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
#include "stringhelper.h"

namespace keyple {
namespace core {
namespace util {

using namespace keyple::common;

KeypleAssert::KeypleAssert() {}

const KeypleAssert& KeypleAssert::getInstance()
{
    return INSTANCE;
}

const KeypleAssert& KeypleAssert::notNull(void* obj, const std::string& name)
{
    if (obj == nullptr)
        throw IllegalArgumentException("Argument [" + name + "] is null.");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::notEmpty(const std::string& obj,
                                           const std::string& name)
{
    if (obj.empty())
        throw IllegalArgumentException("Argument [" + name + "] is empty.");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::notEmpty(const std::vector<uint8_t> obj,
                                           const std::string& name)
{
    if (obj.empty())
        throw IllegalArgumentException("Argument [" + name + "] is empty.");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::isTrue(const bool condition,
                                         const std::string& name)
{
    if (!condition)
        throw IllegalArgumentException("Condition [" + name + "] is false.");

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::greaterOrEqual(const int number,
                                                 const int minValue,
                                                 const std::string& name)
{
    if (number < minValue)
        throw IllegalArgumentException(
                  StringHelper::formatSimple(
                      "Argument [%s] has a value [%d] less than [%d].",
                      name,
                      number,
                      minValue));

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::isEqual(const int number,
                                          const int value,
                                          const std::string& name)
{
    if (number != value)
        throw IllegalArgumentException(
                  StringHelper::formatSimple(
                      "Argument [%s] has a value [%d] not equal to [%d].",
                       name,
                       number,
                       value));

    return INSTANCE;
}

const KeypleAssert& KeypleAssert::isInRange(const int number,
                                            const int minValue,
                                            const int maxValue,
                                            const std::string& name)
{
    if (number < minValue)
        throw IllegalArgumentException(
                  StringHelper::formatSimple(
                      "Argument [%s] has a value [%d] less than [%d].",
                      name,
                      number,
                      minValue));

    else if (number > maxValue)
        throw IllegalArgumentException(
                  StringHelper::formatSimple(
                      "Argument [%s] has a value [%d] more than [%d].",
                      name,
                      number,
                      maxValue));

    return INSTANCE;
}

}
}
}
