/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

#pragma once

#include <memory>
#include <mutex>
#include <typeinfo>
#include <vector>

/* Common*/
#include "Export.h"
#include "Logger.h"

namespace keyple {
namespace common {

class EXPORT LoggerFactory {
public:
    /**
     * Mutex for critical sections (std::cout usage)
     */
    static std::mutex mtx;

    /**
     *
     */
    static std::shared_ptr<Logger> getLogger(const std::type_info &type);
};

}
}
