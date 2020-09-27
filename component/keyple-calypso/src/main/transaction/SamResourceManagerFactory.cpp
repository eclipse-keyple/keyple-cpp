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

#include "SamResourceManagerFactory.h"

namespace keyple {
namespace calypso {
namespace transaction {

std::shared_ptr<SamResourceManager> SamResourceManagerFactory::instantiate(
    const std::shared_ptr<ReaderPlugin> readerPlugin,
    const std::string& samReaderFilter,
    const int maxBlockingTime,
    const int sleepTime)
{
    return std::make_shared<SamResourceManagerDefault>(readerPlugin,
                                                       samReaderFilter,
                                                       maxBlockingTime,
                                                       sleepTime);
}

std::shared_ptr<SamResourceManager> SamResourceManagerFactory::instantiate(
    const std::shared_ptr<ReaderPlugin> readerPlugin,
    const std::string& samReaderFilter)
{
    return std::make_shared<SamResourceManagerDefault>(readerPlugin,
                                                       samReaderFilter,
                                                       MAX_BLOCKING_TIME,
                                                       DEFAULT_SLEEP_TIME);
}

std::shared_ptr<SamResourceManager> SamResourceManagerFactory::instantiate(
    const ReaderPoolPlugin& samReaderPoolPlugin,
    const int maxBlockingTime,
    const int sleepTime)
{
        return std::make_shared<SamResourceManagerPool>(samReaderPoolPlugin,
                                                        maxBlockingTime,
                                                        sleepTime);
}

std::shared_ptr<SamResourceManager> SamResourceManagerFactory::instantiate(
        const ReaderPoolPlugin& samReaderPoolPlugin)
{
    return std::make_shared<SamResourceManagerPool>(samReaderPoolPlugin,
                                                    MAX_BLOCKING_TIME,
                                                    DEFAULT_SLEEP_TIME);
}

}
}
}
