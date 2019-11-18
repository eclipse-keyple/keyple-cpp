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

#include "Logger.h"
#include "LoggerFactory.h"

namespace keyple {
namespace common {

std::mutex LoggerFactory::mtx;

std::shared_ptr<Logger> LoggerFactory::getLogger(const std::type_info &type)
{
    Logger *logger = new Logger(type.name(), &mtx);
    return std::shared_ptr<Logger>(logger);
}

}
}
