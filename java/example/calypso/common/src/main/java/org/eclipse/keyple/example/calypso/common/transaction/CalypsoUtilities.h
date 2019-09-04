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

#include <memory>
#include <string>

/* Calypso */
#include "PoTransaction.h"

/* Common */
#include "exceptionhelper.h"
#include "Properties.h"

/* Core */
#include "SeProxyService.h"


namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace transaction {

using namespace org::eclipse::keyple::calypso::transaction;
using namespace org::eclipse::keyple::common;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::exception;

class CalypsoUtilities : public std::enable_shared_from_this<CalypsoUtilities> {
private:
    /**
     *
     */
    static std::shared_ptr<Properties> properties;

    /**
     *
     */
    class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
    public:
        StaticConstructor();
    };

    /**
     *
     */
    static CalypsoUtilities::StaticConstructor staticConstructor;

public:
    /**
     * Get the default reader for PO communications
     *
     * @param seProxyService the current ProxyService
     * @return a SeReader object
     * @throws KeypleBaseException if an error occurred
     */
    static std::shared_ptr<SeReader> getDefaultPoReader();

    /**
     * Get the default reader for SAM communications
     *
     * @param seProxyService the current ProxyService
     * @return a {@link SamResource} object
     * @throws KeypleBaseException if an error occurred
     */
    static std::shared_ptr<SamResource> getDefaultSamResource();

    /**
     *
     */
    static std::shared_ptr<SecuritySettings> getSecuritySettings();

    /**
     * Check SAM presence and consistency and return a SamResource when everything is correct
     * <p>
     * Throw an exception if the expected SAM is not available
     *
     * @param samReader the SAM reader
     */
    static std::shared_ptr<SamResource> checkSamAndOpenChannel(std::shared_ptr<SeReader> samReader);
};

}
}
}
}
}
}
}
