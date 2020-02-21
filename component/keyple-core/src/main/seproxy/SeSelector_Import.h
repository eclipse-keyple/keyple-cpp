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

#include <set>
#include <string>
#include <unordered_set>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <memory>

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "ApduRequest.h"
#include "ChannelControl.h"
#include "SeProtocol_Import.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::common;

/**
 * The SeSelector class groups the information and methods used to select a
 * particular secure element
 */
class IMPORT SeSelector : public std::enable_shared_from_this<SeSelector> {
public:
    /**
     * Static nested class to hold the data elements used to perform an AID
     * based selection
     */
    class IMPORT AidSelector : public std::enable_shared_from_this<AidSelector>{
    public:
        /**
         * FileOccurrence indicates how to carry out the file occurrence in
         * accordance with ISO7816-4
         * <p>
         * The getIsoBitMask method provides the bit mask to be used to set P2
         * in the select command (ISO/IEC 7816-4.2)
         */
        class IMPORT FileOccurrence final {
        public:
            static FileOccurrence FIRST;
            static FileOccurrence LAST;
            static FileOccurrence NEXT;
            static FileOccurrence PREVIOUS;

            /**
             *
             */
            enum class InnerEnum {
                FIRST,
                LAST,
                NEXT,
                PREVIOUS
            };

            /**
             *
             */
            const InnerEnum innerEnumValue;

            /**
             *
             */
            FileOccurrence(const std::string &name, InnerEnum innerEnum, char
                           isoBitMask);

            /**
             *
             */
            FileOccurrence(const FileOccurrence& o);

            /**
             *
             */
            virtual ~FileOccurrence() {}

            /**
             *
             */
            virtual char getIsoBitMask();

            /**
             *
             */
            bool operator == (const FileOccurrence &other);

            /**
             *
             */
            bool operator != (const FileOccurrence &other);

            /**
             *
             */
            static std::vector<FileOccurrence> values();

            /**
             *
             */
            int ordinal();

            /**
             *
             */
            std::string toString();

            /**
             *
             */
            static FileOccurrence valueOf(const std::string &name);

        private:
            /**
             *
             */
            static std::vector<FileOccurrence> valueList;

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
            static StaticConstructor staticConstructor;

            /**
             *
             */
            const std::string nameValue;

            /**
             *
             */
            const int ordinalValue;

            /**
             *
             */
            static int nextOrdinal;

            /**
             *
             */
            char isoBitMask;
        };

        /**
         * FileOccurrence indicates how to which template is expected in
         * accordance with ISO7816-4
         * <p>
         * The getIsoBitMask method provides the bit mask to be used to set P2
         * in the select command (ISO/IEC 7816-4.2)
         */
        class IMPORT FileControlInformation final {
        public:
            static FileControlInformation FCI;
            static FileControlInformation FCP;
            static FileControlInformation FMD;
            static FileControlInformation NO_RESPONSE;

            /**
             *
             */
            enum class InnerEnum {
                FCI,
                FCP,
                FMD,
                NO_RESPONSE
            };public:

            /**
             *
             */
            FileControlInformation(const std::string &name, InnerEnum innerEnum,
                                   char isoBitMask);

            /**
             *
             */
            FileControlInformation(const FileControlInformation& o);

            /**
             *
             */
            virtual ~FileControlInformation() {}

            /**
             *
             */
            virtual char getIsoBitMask();

            /**
             *
             */
            bool operator == (const FileControlInformation &other);

            /**
             *
             */
            bool operator != (const FileControlInformation &other);

            /**
             *
             */
            static std::vector<FileControlInformation> values();

            /**
             *
             */
            int ordinal();

            /**
             *
             */
            std::string toString();

            /**
             *
             */
            static FileControlInformation valueOf(const std::string &name);

        private:
            /**
             *
             */
            static std::vector<FileControlInformation> valueList;

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
            static StaticConstructor staticConstructor;

            /**
             *
             */
            const InnerEnum innerEnumValue;

            /**
             *
             */
            const std::string nameValue;

            /**
             *
             */
            const int ordinalValue;

            /**
             *
             */
            static int nextOrdinal;

            /**
             *
             */
            char isoBitMask;
        };

        /**
         *
         */
        class IMPORT IsoAid : public std::enable_shared_from_this<IsoAid> {
        public:
            /**
             *
             */
            static constexpr int AID_MIN_LENGTH = 5;
            static constexpr int AID_MAX_LENGTH = 16;

            /**
             * Build an IsoAid and check length from a byte array
             *
             * @param aid byte array containing the AID value
             * @throws IllegalArgumentException if the byte length array is not
             *         within the allowed range.
             */
            IsoAid(const std::vector<uint8_t>& aid);


            /**
             * Build an IsoAid and check length from an hex string
             *
             * @param aid hex string containing the AID value
             * @throws IllegalArgumentException if the byte length array is not
             *         within the allowed range.
             */
            IsoAid(const std::string& aid);

            /**
             *
             */
            virtual ~IsoAid() {}

            /**
             * @return the AID value as a byte array
             */
            virtual const std::vector<uint8_t>& getValue();

            /**
                * Compares two IsoAid objects.
                * <p>
                * Tells if the current AID starts with the value contained in
                * the provided AID
                *
                * @param aid an other AID
                * @return true or false
                */
            virtual bool startsWith(std::shared_ptr<IsoAid> aid);

        private:
            /**
             *
             */
            std::vector<uint8_t> value;


        };


    public:
        /**
         * AidSelector with additional select application successful status
         * codes, file occurrence and file control information.
         * <p>
         * The fileOccurrence parameter defines the selection options P2 of the
         * SELECT command message
         * <p>
         * The fileControlInformation parameter defines the expected command
         * output template.
         * <p>
         * Refer to ISO7816-4.2 for detailed information about these parameters
         *
         * @param aidToSelect IsoAid
         * @param successfulSelectionStatusCodes list of successful status codes
         *        for the select application response
         * @param fileOccurrence the occurrence parameter (see ISO7816-4
         *        definition)
         * @param fileControlInformation the file control information (see
         *        ISO7816-4 definition)
         */
        AidSelector(std::shared_ptr<IsoAid> aidToSelect,
                    std::shared_ptr<std::set<int>>
                        successfulSelectionStatusCodes,
                    FileOccurrence fileOccurrence,
                    FileControlInformation fileControlInformation);

        /**
         * AidSelector with additional select application successful status
         * codes
         * <p>
         * The fileOccurrence field is set by default to FIRST
         * <p>
         * The fileControlInformation field is set by default to FCI
         *
         * @param aidToSelect IsoAid
         * @param successfulSelectionStatusCodes list of successful status codes
         *        for the select application response
         */
        AidSelector(std::shared_ptr<IsoAid> aidToSelect,
                    std::shared_ptr<std::set<int>>
                        successfulSelectionStatusCodes);

        /**
         * Destructor
         */
        virtual ~AidSelector() {}

        /**
         * Getter for the AID provided at construction time
         *
         * @return byte array containing the AID
         */
        virtual std::shared_ptr<IsoAid> getAidToSelect();

        /**
         * @return the file occurrence parameter
         */
        virtual FileOccurrence getFileOccurrence();

        /**
         * @return the file control information parameter
         */
        virtual FileControlInformation getFileControlInformation();

        /**
         * Gets the list of successful selection status codes
         *
         * @return the list of status codes
         */
        virtual std::shared_ptr<std::set<int>>
            getSuccessfulSelectionStatusCodes();


        /**
         * Print out the AID in hex
         *
         * @return a string
         */
        std::string toString();

    private:
        /**
         *
         */
        FileOccurrence fileOccurrence = FileOccurrence::FIRST;

        /**
         *
         */
        FileControlInformation fileControlInformation =
            FileControlInformation::FCI;

        /**
         * - AID’s bytes of the SE application to select. In case the SE
         * application is currently not selected, a logical channel is
         * established and the corresponding SE application is selected by the
         * SE reader, otherwise keep the current channel.
         *
         * - Could be missing when operating SE which don’t support the Select
         * Application command (as it is the case for SAM).
         */
        std::shared_ptr<IsoAid> aidToSelect;

        /**
         * List of status codes in response to the select application command
         * that should be considered successful although they are different from
         * 9000
         */
        std::shared_ptr<std::set<int>> successfulSelectionStatusCodes =
            std::make_shared<std::set<int>>();

    };

    /**
     * Static nested class to hold the data elements used to perform an ATR
     * based filtering
     */
    class IMPORT AtrFilter : public std::enable_shared_from_this<AtrFilter> {
    public:
        /**
         * Regular expression based filter
         *
         * @param atrRegex String hex regular expression
         */
        AtrFilter(const std::string &atrRegex);

        /**
         *
         */
        virtual ~AtrFilter() {};

        /**
         * Setter for the regular expression provided at construction time
         *
         * @param atrRegex expression string
         */
        virtual void setAtrRegex(const std::string &atrRegex);

        /**
        * Getter for the regular expression provided at construction time
        *
        * @return Regular expression string
        */
        virtual std::string getAtrRegex();

        /**
        * Tells if the provided ATR matches the registered regular expression
        *
        * If the registered regular expression is empty, the ATR is always
        * matching.
        *
        * @param atr a buffer containing the ATR to be checked
        * @return a boolean true the ATR matches the current regex
        */
        virtual bool atrMatches(const std::vector<uint8_t>& atr);

        /**
        * Print out the ATR regex
        *
        * @return a string
        */
        std::string toString();

    private:
        /**
         * Regular expression dedicated to handle SE logical channel opening
         * based on ATR pattern
         */
        std::string atrRegex;
    };



public:
    /**
     * Create a SeSelector to perform the SE selection
     * <p>
     * if seProtocol is null, all protocols will match and the selection process
     * will continue
     *
     * <p>
     * if seProtocol is not null, the current SE protocol will checked and the
     * selection process will continue only if the protocol matches.
     *
     * <p>
     * if aidSelector is null, no 'select application' command is generated. In
     * this case the SE must have a default application selected. (e.g. SAM or
     * Rev1 Calypso cards)
     * <p>
     * if aidSelector is not null, a 'select application' command is generated
     * and performed. Furthermore, the status code is checked against the list
     * of successful status codes in the {@link AidSelector} to determine if the
     * SE matched or not the selection data.
     * <p>
     * if atrFilter is null, no check of the ATR is performed. All SE will
     * match.
     * <p>
     * if atrFilter is not null, the ATR of the SE is compared with the regular
     * expression provided in the {@link AtrFilter} in order to determine if the
     * SE match or not the expected ATR.
     *
     * @param seProtocol the SE communication protocol
     * @param atrFilter the ATR filter
     * @param aidSelector the AID selection data
     * @param extraInfo information string (to be printed in logs)
     */
    SeSelector(SeProtocol& seProtocol, std::shared_ptr<AtrFilter> atrFilter,
               std::shared_ptr<AidSelector> aidSelector,
               const std::string &extraInfo);

    /**
     *
     */
    virtual ~SeSelector() {}

    /**
     * Getter
     *
     * @return the {@link SeProtocol} provided at construction time
     */
    virtual const SeProtocol& getSeProtocol();

    /**
     * Getter
     *
     * @return the {@link AtrFilter} provided at construction time
     */
    virtual std::shared_ptr<AtrFilter> getAtrFilter();

    /**
     * Getter
     *
     * @return the {@link AidSelector} provided at construction time
     */
    virtual std::shared_ptr<AidSelector> getAidSelector();

    /**
     * Gets the information string
     *
     * @return a string to be printed in logs
     */
    std::string getExtraInfo();

    /**
     *
     */
    std::string toString();

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(SeSelector));

    /**
     *
     */
    const SeProtocol seProtocol;

    /**
     *
     */
    const std::shared_ptr<AidSelector> aidSelector;

    /**
     *
     */
    const std::shared_ptr<AtrFilter> atrFilter;

    /**
     *
     */
    const std::string extraInfo;
};

}
}
}
