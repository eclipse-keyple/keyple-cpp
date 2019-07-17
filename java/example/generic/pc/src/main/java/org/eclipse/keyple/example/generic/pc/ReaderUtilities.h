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
#include <memory>

#include "SeProxyService.h"
#include "SeReader.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace pc {

using namespace org::eclipse::keyple::core::seproxy;

class ReaderUtilities : public std::enable_shared_from_this<ReaderUtilities> {
public:
    /**
     * Get the terminal which names match the expected pattern
     *
     * @param seProxyService SE Proxy service
     * @param pattern Pattern
     * @return SeReader
     * @throws KeypleReaderException Readers are not initialized
     */
    static std::shared_ptr<SeReader> getReaderByName(std::shared_ptr<SeProxyService> seProxyService, const std::string &pattern);

    /**
     * Get a fully configured contactless proxy reader
     * 
     * @param seProxyService the current SeProxyService
     * @return the targeted SeReader to do contactless communications
     * @throws KeypleBaseException in case of an error while retrieving the reader or setting its
     *         parameters
     */
    static std::shared_ptr<SeReader> getDefaultContactLessSeReader(std::shared_ptr<SeProxyService> seProxyService);

    /**
     * Sets the reader parameters for contactless secure elements
     * 
     * @param reader the reader to configure
     * @throws KeypleBaseException in case of an error while settings the parameters
     */
    static void setContactlessSettings(std::shared_ptr<SeReader> reader);

    /**
     * Sets the reader parameters for contacts secure elements
     *
     * @param reader the reader to configure
     * @throws KeypleBaseException in case of an error while settings the parameters
     */
    static void setContactsSettings(std::shared_ptr<SeReader> reader);
};

}
}
}
}
}
}
