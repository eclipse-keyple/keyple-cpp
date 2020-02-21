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

#include <string>
#include <unordered_map>
#include <memory>

/* Common */
#include "Export.h"
#include "exceptionhelper.h"

/* Core */
#include "ApduResponse.h"

namespace keyple {
namespace core {
namespace command {

using namespace keyple::core::seproxy::message;

/**
    * Base class for parsing APDU
    */
class EXPORT AbstractApduResponseParser
: public std::enable_shared_from_this<AbstractApduResponseParser> {
public:
    /**
     * Status code properties
     */
    class EXPORT StatusProperties
    : public std::enable_shared_from_this<StatusProperties> {
    public:
        /**
         * A map with the double byte of a status as key, and the successful property and ASCII text
         * information as data.
         *
         * @param successful set successful status
         * @param information additional information
         */
        StatusProperties(bool successful, const std::string& information);

        /**
         *
         */
        virtual ~StatusProperties()
        {
        }

        /**
         * Gets the successful.
         *
         * @return the successful
         */
        virtual bool isSuccessful();

        /**
         * Gets the information.
         *
         * @return the information
         */
        virtual std::string getInformation();

    private:
        /**
         * The successful
         */
        const bool successful;

        /**
         * The information
         */
        const std::string information;
    };

    /**
     * the generic abstract constructor to build a parser of the APDU response.
     *
     * @param response response to parse
     */
    AbstractApduResponseParser(std::shared_ptr<ApduResponse> response);

    /**
     * Default constructor
     */
    AbstractApduResponseParser();

    /**
     *
     */
    virtual ~AbstractApduResponseParser()
    {
    }

    /**
     * Sets the Apdu response to parse
     *
     * @param response the apdu response
     */
    void setApduResponse(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    bool isInitialized();

    /**
     * Gets the apdu response.
     *
     * @return the ApduResponse instance.
     */
    const std::shared_ptr<ApduResponse> getApduResponse() const;

    /**
     * Checks if is successful.
     *
     * @return if the status is successful from the statusTable according to the current status
     *         code.
     */
    virtual bool isSuccessful() const;

    /**
     * Gets the status information.
     *
     * @return the ASCII message from the statusTable for the current status code.
     */
    std::string getStatusInformation();

protected:
    /**
     * The byte array APDU response
     */
    std::shared_ptr<ApduResponse> response;

    /**
     *
     */
    static std::unordered_map<int, std::shared_ptr<StatusProperties>>
        STATUS_TABLE;

    /**
     * Get the internal status table
     *
     * @return Status table
     */
    virtual std::unordered_map<int, std::shared_ptr<StatusProperties>>
    getStatusTable() const;

private:
    /**
     *
     */
    class StaticConstructor
    : public std::enable_shared_from_this<StaticConstructor> {
    public:
        StaticConstructor();
    };

    /**
     *
     */
    static AbstractApduResponseParser::StaticConstructor staticConstructor;

    /**
     * Indicates whether the ApduResponse has been provided or not
     */
    bool initialized = false;

    // Note: The conversion of all commands was done with:
    // Input regex: new byte\[\] \{\(byte\) 0x([0-9A-Za-z]{2})\, \(byte\) 0x([0-9A-Za-z]{2})\}
    // Output regex: 0x$1$2

    /**
     *
     */
    int getStatusCode() const;

    /**
     *
     */
    std::shared_ptr<StatusProperties> getPropertiesForStatusCode() const;
};

}
}
}
