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

#include "NoStackTraceThrowable.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

NoStackTraceThrowable::NoStackTraceThrowable()
: std::runtime_error("")
{
}

NoStackTraceThrowable::NoStackTraceThrowable(const std::string &message)
: std::runtime_error(message)
{
}

std::runtime_error NoStackTraceThrowable::fillInStackTrace()
{
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to
// native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
//                        synchronized(shared_from_this()) {
//                            return shared_from_this();
//                        }
    return std::runtime_error("NoStackTraceThrowable");
}

}
}
}
}
