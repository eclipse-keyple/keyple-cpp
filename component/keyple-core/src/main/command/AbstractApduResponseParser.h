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

#pragma once

#include <string>
#include <map>
#include <memory>

/* Common */
#include "Integer.h"
#include "KeypleCoreExport.h"

/* Core */
#include "AbstractApduCommandBuilder.h"
#include "ApduResponse.h"
#include "KeypleSeCommandException.h"

namespace keyple {
namespace core {
namespace command {

using namespace keyple::core::command;
using namespace keyple::core::command::exception;
using namespace keyple::core::seproxy::message;

/**
 * Base class for parsing APDU
 */
class KEYPLECORE_API AbstractApduResponseParser
: public std::enable_shared_from_this<AbstractApduResponseParser> {
public:
    /**
     * Status code properties
     */
    class KEYPLECORE_API StatusProperties
    : public std::enable_shared_from_this<StatusProperties> {
    public:
        /**
         * Create a successful status.
         *
         * @param information the status information
         */
        StatusProperties(const std::string& information);

        /**
         * Create an error status.<br>
         * If {@code exceptionClass} is null, then a successful status is
         * created.
         *
         * @param information the status information
         * @param exceptionClass the associated exception class
         */
        StatusProperties(const std::string& information,
                         const std::type_info& exceptionClass);

        /**
         *
         */
        virtual ~StatusProperties() {}

        /**
         * Gets the successful.
         *
         * @return the successful
         */
        virtual bool isSuccessful() const;

        /**
         * @return the status information
         */
        virtual const std::string& getInformation() const;

        /**
         * @return the nullable exception class
         */
        virtual const std::type_info&  getExceptionClass() const;

    private:
        /**
         * The successful indicator
         */
        const bool mSuccessful;

        /**
         * The status information
         */
        const std::string mInformation;

        /**
         * The associated exception class in case of error status
         */
        const std::type_info& mExceptionClass;
    };

    /**
     * the generic abstract constructor to build a parser of the APDU response.
     *
     * @param response response to parse
     * @param builder the reference of the builder that created the parser
     */
    AbstractApduResponseParser(
        const std::shared_ptr<ApduResponse> response,
        const std::shared_ptr<AbstractApduCommandBuilder> builder);

    /**
     *
     */
    virtual ~AbstractApduResponseParser() {}

    /**
     * Sets the Apdu response to parse
     *
     * @param response the apdu response
     */
    void setApduResponse(const std::shared_ptr<ApduResponse>& response);

    /**
     * Gets the apdu response.
     *
     * @return the APDU response.
     */
    const std::shared_ptr<ApduResponse> getApduResponse() const;

    /**
     * @return the associated builder reference
     */
    const std::shared_ptr<AbstractApduCommandBuilder> getBuilder() const;

    /**
     * @return true if the status is successful from the statusTable according
     *         to the current status code.
     */
    virtual bool isSuccessful() const;

    /**
     * @return the ASCII message from the statusTable for the current status
     *         code.
     */
    std::string getStatusInformation() const;

    /**
     * This method check the status code.<br>
     * If status code is not referenced, then status is considered unsuccessful.
     *
     * @throws KeypleSeCommandException if status is not successful.
     */
    void checkStatus();

protected:
    /**
     * The byte array APDU response
     */
    std::shared_ptr<ApduResponse> mResponse;

    /**
     * Parsers are usually created by their associated builder. The CalypsoSam
     * field maintains a link between the builder and the parser in order to
     * allow the parser to access the builder parameters that were used to
     * create the command (e.g. SFI, registration number, etc.).
     */
    const std::shared_ptr<AbstractApduCommandBuilder> mBuilder;

    /**
     *
     */
    static std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;

    /**
     * Build a command exception.<br>
     * This method should be override in subclasses in order to create specific
     * exceptions.
     *
     * @param exceptionClass the exception class
     * @param message the message
     * @param commandRef the command reference
     * @param statusCode the status code
     * @return a new instance not null
     */
    const std::shared_ptr<KeypleSeCommandException> buildCommandException(
            const std::type_info& exceptionClass,
            const std::string& message,
            const std::shared_ptr<SeCommand> commandRef,
            const std::shared_ptr<Integer> statusCode);

    /**
     * @return the internal status table
     */
    virtual std::map<int, std::shared_ptr<StatusProperties>>&
        getStatusTable() const;

    /**
     * Gets the associated command reference.<br>
     * By default, the command reference is retrieved from the associated builder.
     *
     * @return a nullable command reference
     */
    virtual const std::shared_ptr<SeCommand> getCommandRef() const;

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
     * @return the properties associated to the response status code
     */
    const std::shared_ptr<StatusProperties> getStatusCodeProperties() const;
};

}
}
}
