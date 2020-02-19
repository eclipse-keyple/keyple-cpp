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

#include "StubSecureElement.h"

namespace keyple {
namespace plugin {
namespace stub {

class StubReader : public ObservableReader {
public:
    const std::string ALLOWED_PARAMETER_1 = "parameter1";
    const std::string ALLOWED_PARAMETER_2 = "parameter2";
    const std::string CONTACTLESS_PARAMETER = "contactless";
    const std::string CONTACTS_PARAMETER = "contacts";

    /**
     * Insert a stub se into the reader. Will raise a SE_INSERTED event.
     *
     * @param _se stub secure element to be inserted in the reader
     */
    void insertSe(std::shared_ptr<StubSecureElement> _se);

    /**
     * Remove se from reader if any
     */
    void removeSe();

    /**
     * Get inserted SE
     *
     * @return se, can be null if no Se inserted
     */
    std::shared_ptr<StubSecureElement> getSe();
};

}
}
}