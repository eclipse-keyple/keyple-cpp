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

#include "Pattern.h"

namespace keyple {
namespace common {

Pattern::Pattern(const std::string& pattern, const int flags) : mPattern(pattern), mFlags(flags) {}

std::unique_ptr<Pattern> Pattern::compile(const std::string& regularExpression, const int flags)
    const
{
    /* Compiler hack */
    (void)mFlags;

    return std::unique_ptr<Pattern>(new Pattern(regularExpression, flags));
}

std::unique_ptr<Pattern> Pattern::compile(const std::string& pattern)
{
    return std::unique_ptr<Pattern>(new Pattern(pattern, 0));
}

std::unique_ptr<Matcher> Pattern::matcher(const std::string& input) const
{
    return std::unique_ptr<Matcher>(new Matcher(this, input));
}

}
}
