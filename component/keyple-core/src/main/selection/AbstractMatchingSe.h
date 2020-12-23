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
#include <vector>

/* Core */
#include "SeResponse.h"
#include "TransmissionMode.h"

/* Common */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;

/**
 * AbstractMatchingSe is the class to manage the elements of the result of a
 * selection.<br>
 * This class should be extended for the management of specific SE.<br>
 * Nevertheless it gives access to the generic parameters common to all SEs
 * which are the transmission mode, the FCI (response to select command) and the
 * ATR (SE's answer to reset) when they are available.
 */
class KEYPLECORE_API AbstractMatchingSe
: public std::enable_shared_from_this<AbstractMatchingSe> {
public:
    /**
     *
     */
    virtual ~AbstractMatchingSe() {}

    /**
     * @return the SE keyple::core::seproxy::protocol::TransmissionMode (contacts or contactless)
     */
    virtual const TransmissionMode& getTransmissionMode() const;

    /**
     * @return true if the matching SE has an FCI
     */
    bool hasFci() const;

    /**
     * @return true if the matching SE has an ATR
     */
    bool hasAtr() const;

    /**
     * @return the FCI
     * @throws IllegalStateException if no FCI is available (see hasFci)
     */
    const std::vector<uint8_t>& getFciBytes() const;
    /**
     * @return the ATR
     * @throws IllegalStateException if no ATR is available (see hasAtr)
     */
    const std::vector<uint8_t>& getAtrBytes() const;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const AbstractMatchingSe& ams);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const std::shared_ptr<AbstractMatchingSe>& ams);

    /**
     * Constructor
     *
     * C++ vs. Java: constructor is protected in Java but prevents use of std::make_shared in C++.
     *
     * @param selectionResponse the response from the SE
     * @param transmissionMode the transmission mode, contact or contactless
     */
    AbstractMatchingSe(const std::shared_ptr<SeResponse> selectionResponse,
                       const TransmissionMode& transmissionMode);

private:
    /**
     *
     */
    std::vector<uint8_t> mFciBytes;

    /**
     *
     */
    std::vector<uint8_t> mAtrBytes;

    /**
     *
     */
    const TransmissionMode mTransmissionMode;
};

}
}
}
