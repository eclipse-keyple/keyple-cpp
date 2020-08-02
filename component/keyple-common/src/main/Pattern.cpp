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

#include "Pattern.h"

namespace keyple {
namespace common {

Pattern::Pattern(std::string pattern, int flags)
: pattern(pattern), mFlags(flags)
{
}

Pattern* Pattern::compile(std::string regularExpression, int flags)
{
    /* Compiler hack */
    (void)mFlags;

    return new Pattern(regularExpression, flags);
}

Pattern* Pattern::compile(std::string pattern)
{
    return new Pattern(pattern, 0);
}

Matcher* Pattern::matcher(std::string input)
{
    return new Matcher(this, input);
}

}
}
