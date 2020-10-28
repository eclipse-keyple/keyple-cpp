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

#pragma once

#include <memory>
#include <string>

/* Calypso */
#include "CalypsoSam.h"
#include "PoTransaction.h"

/* Common */
#include "LoggerFactory.h"
#include "Properties.h"

/* Core */
#include "SeProxyService.h"

namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace transaction {

using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;

class CalypsoUtilities final {
public:
    /**
     * Get the default reader for PO communications
     *
     * @param seProxyService the current ProxyService
     * @return a SeReader object
     * @throw KeypleException if an error occurred
     */
    static std::shared_ptr<SeReader> getDefaultPoReader();

    /**
     * Get the default reader for SAM communications
     *
     * @param seProxyService the current ProxyService
     * @return a {@link SeResource} object
     * @throw KeypleException if an error occurred
     */
    static std::shared_ptr<SeResource<CalypsoSam>> getDefaultSamResource();

    /**
     *
     */
    static std::shared_ptr<PoSecuritySettings> getSecuritySettings(
        std::shared_ptr<SeResource<CalypsoSam>> samResource);

    /**
     * Check SAM presence and consistency and return a {@code SeResource<CalypsoSam>} when
     * everything is correct.
     * <p>
     * Throw an exception if the expected SAM is not available
     *
     * @param samReader the SAM reader
     * @return the SAM SeResource
     */
    static std::shared_ptr<SeResource<CalypsoSam>> checkSamAndOpenChannel(
        std::shared_ptr<SeReader> samReader);

private:
    /**
     *
     */
    static std::shared_ptr<Properties> properties;

    /**
     *
     */
    class StaticConstructor {
    public:
        StaticConstructor();
    };

    /**
     *
     */
    CalypsoUtilities();

    /**
     *
     */
    static CalypsoUtilities::StaticConstructor staticConstructor;

    /**
     *
     */
    static const std::shared_ptr<Logger> mLogger;
};

}
}
}
}
}
