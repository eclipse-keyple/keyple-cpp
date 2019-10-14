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

#include <stdexcept>
#include <memory>

/* Common */
#include "Export.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
    * Exception that do not print stack trace Useful when the exceptions are expected and managed.
    */
// TODO workaround for no stackstrace; should we keep it?
class EXPORT NoStackTraceThrowable : public std::runtime_error, public std::enable_shared_from_this<NoStackTraceThrowable> {

public:
    NoStackTraceThrowable();

    NoStackTraceThrowable(const std::string &message);

    std::runtime_error fillInStackTrace();// override;
};

}
}
}
}
}
}
