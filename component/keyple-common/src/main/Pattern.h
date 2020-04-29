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

#include <string>

#include "KeypleCommonExport.h"
#include "Matcher.h"

namespace keyple {
namespace common {

class KEYPLECOMMON_API Pattern {
public:
    /*
	 *
	 */
    std::string pattern;

    /*
	 *
	 */
    int flags;

public:
    /*
	 * Constructor
	 */
    Pattern(std::string pattern, int flags);

    /*
	 * Returns a compiled form of the given regular expression, as modified by the
	 * given flags.
	 * 
	 * @throws PatternSyntexException
	 */
    Pattern* compile(std::string regularExpression, int flags);

    /*
	 * Equivalent to Pattern.compile(pattern, 0)
	 */
    static Pattern* compile(std::string pattern);

    /*
	 * Returns a Matcher for this pattern applied to the given input. The Matcher can
	 * be used to match the Pattern against the whole input, find occurences of the
	 * Pattern in the input, or replace parts of the input.
	 */
    Matcher* matcher(std::string input);
};

}
}
