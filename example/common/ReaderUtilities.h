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

#include <string>
#include <memory>

#include "SeProxyService.h"
#include "SeReader.h"

namespace keyple {
namespace example {
namespace common {

using namespace keyple::core::seproxy;

class ReaderUtilities final {
public:
    /**
     * Get the terminal which names match the expected pattern
     *
     * @param pattern Pattern
     * @return SeReader
     * @throw KeypleReaderException Readers are not initialized
     */
    static std::shared_ptr<SeReader> getReaderByName(const std::string& pattern);

    /**
     * Get a fully configured contactless proxy reader
     *
     * @param seProxyService the current SeProxyService
     * @return the targeted SeReader to do contactless communications
     * @throw KeypleException in case of an error while retrieving the reader or setting its
     *        parameters
     */
    static std::shared_ptr<SeReader> getDefaultContactLessSeReader();

    /**
     * Sets the reader parameters for contactless secure elements
     *
     * @param reader the reader to configure
     * @throw KeypleException in case of an error while settings the parameters
     */
    static void setContactlessSettings(std::shared_ptr<SeReader> reader);

    /**
     * Sets the reader parameters for contacts secure elements
     *
     * @param reader the reader to configure
     * @throw KeypleException in case of an error while settings the parameters
     */
    static void setContactsSettings(std::shared_ptr<SeReader> reader);

private:
    /**
     *
     */
    ReaderUtilities();
};

}
}
}
