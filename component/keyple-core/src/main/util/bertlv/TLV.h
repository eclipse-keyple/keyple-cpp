/********************************************************************************
* Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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
#include <vector>
#include <stdexcept>
#include <memory>

/* Common */
#include "Export.h"

/* Forward class declarations */
namespace keyple { namespace core { namespace util { namespace bertlv { class Tag; } } } }

namespace keyple {
namespace core {
namespace util {
namespace bertlv {


/**
 * This class helps to parse a byte array as a TLV structure
 * <p>
 * (ITU-T X.690 / ISO 8825)
 */
class EXPORT TLV : public std::enable_shared_from_this<TLV> {
public:
    /**
     * Create a TLV object initialized with a byte array
     * <p>
     *
     * @param binary the byte array containing the TLV structure
     */
    TLV(std::vector<char> &binary);

    /**
     *
     */
    virtual ~TLV() {}

    /**
     * Parse the byte array to find the expected TLV.
     * <p>
     * The method returns true if the tag is found.
     * <p>
     * The analysis result is available with getValue and getPosition
     *
     * @param tag the tag to search in the byte array
     * @param offset the position to start in the byte array
     * @return true or false according to the presence of the provided tag
     */
    virtual bool parse(std::shared_ptr<Tag> tag, int offset);

    /**
     * Return a byte array copied from the main array corresponding to value part of the last TLV
     * parsing.
     * <p>
     * This method modifies the global position in the main array. Thus, it must be called once
     * only.
     *
     * @return a byte array
     */
    virtual std::vector<char> getValue();

    /**
     * @return the current position in the main array
     */
    virtual int getPosition();

    /**
     *
     */
    std::string toString();

private:
    /**
     *
     */
    std::shared_ptr<Tag> tag;

    /**
     *
     */
    int length = 0;

    /**
     *
     */
    std::vector<char> binary;

    /**
     *
     */
    int position = 0;

};

}
}
}
}
