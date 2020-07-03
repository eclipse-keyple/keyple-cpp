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
#include "LoggerFactory.h"

/* Core */
#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "ChannelControl.h"
#include "KeypleCoreExport.h"
#include "SeProtocol.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::common;

/**
 * The SeSelector class groups the information and methods used to select a
 * particular secure element
 */
class KEYPLECORE_API SeSelector
: public std::enable_shared_from_this<SeSelector> {
public:
    /**
     * AID’s bytes of the SE application to select. In case the SE application
     * is currently not selected, a logical channel is established and the
     * corresponding SE application is selected by the SE reader, otherwise keep
     * the current channel. - optional {@link FileOccurrence} and
     * {@link FileControlInformation} defines selections modes according to
     * ISO7816-4 - optional successfulSelectionStatusCodes define a list of
     * accepted SW1SW2 codes (in addition to 9000). Allows, for example, to
     * manage the selection of the invalidated cards. - AidSelector could be
     * missing in SeSelector when operating SE which don’t support the Select
     * Application command (as it is the case for SAM).
     */
    class KEYPLECORE_API AidSelector final
    : public std::enable_shared_from_this<AidSelector> {
    public:
        /**
         * FileOccurrence indicates how to carry out the file occurrence in
         * accordance with ISO7816-4
         * <p>
         * The getIsoBitMask method provides the bit mask to be used to set P2
         * in the select command (ISO/IEC 7816-4.2)
         */
        class KEYPLECORE_API FileOccurrence final {
        public:
            static FileOccurrence FIRST;
            static FileOccurrence LAST;
            static FileOccurrence NEXT;
            static FileOccurrence PREVIOUS;

            /**
             *
             */
            enum class InnerEnum { FIRST, LAST, NEXT, PREVIOUS };

            /**
             *
             */
            InnerEnum mInnerEnumValue;

            /**
             *
             */
            FileOccurrence(const std::string& name, InnerEnum innerEnum,
                           char isoBitMask);

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
            bool operator==(const FileOccurrence& other) const;

            /**
             *
             */
            bool operator!=(const FileOccurrence& other) const;

            /**
             *
             */
            FileOccurrence& operator=(const FileOccurrence& other);

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
            static FileOccurrence valueOf(const std::string& name);

            /**
             *
             */
            friend KEYPLECORE_API std::ostream& operator<<(
                std::ostream& os, const FileOccurrence& fc);

        private:
            /**
             *
             */
            static std::vector<FileOccurrence> mValueList;

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
            static StaticConstructor mStaticConstructor;

            /**
             *
             */
            std::string mNameValue;

            /**
             *
             */
            int mOrdinalValue;

            /**
             *
             */
            static int mNextOrdinal;

            /**
             *
             */
            char mIsoBitMask;
        };

        /**
         * FileControlInformation indicates how to which template is expected in
         * accordance with ISO7816-4
         * <p>
         * The getIsoBitMask method provides the bit mask to be used to set P2
         * in the select command (ISO/IEC 7816-4.2)
         */
        class KEYPLECORE_API FileControlInformation final {
        public:
            static FileControlInformation FCI;
            static FileControlInformation FCP;
            static FileControlInformation FMD;
            static FileControlInformation NO_RESPONSE;

            /**
             *
             */
            enum class InnerEnum { FCI, FCP, FMD, NO_RESPONSE };
        public:
            /**
             *
             */
            FileControlInformation(const std::string& name, InnerEnum innerEnum,
                                   char isoBitMask);

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
            bool operator==(const FileControlInformation& other) const;

            /**
             *
             */
            bool operator!=(const FileControlInformation& other) const;

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
            static FileControlInformation valueOf(const std::string& name);

            /**
             *
             */
            friend KEYPLECORE_API std::ostream& operator<<(
                std::ostream& os, const FileControlInformation& fci);

        private:
            /**
             *
             */
            static std::vector<FileControlInformation> mValueList;

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
            static StaticConstructor mStaticConstructor;

            /**
             *
             */
            InnerEnum mInnerEnumValue;

            /**
             *
             */
            std::string mNameValue;

            /**
             *
             */
            int mOrdinalValue;

            /**
             *
             */
            static int mNextOrdinal;

            /**
             *
             */
            char mIsoBitMask;
        };

        /**
         * Builder of {@link AidSelector}
         *
         * @since 0.9
         */
        class AidSelectorBuilder {
        public:
            /**
             * Friend declaration to allow access to private members to
             * AisSelector
             */
            friend AidSelector;

            /**
             * Sets the AID
             *
             * @param aid the AID as an array of bytes
             * @return the builder instance
             */
            AidSelectorBuilder& aidToSelect(const std::vector<uint8_t>& aid);

            /**
             * Sets the AID
             *
             * @param aid the AID as an hex string
             * @return the builder instance
             */
            AidSelectorBuilder& aidToSelect(const std::string& aid);

            /**
             * Sets the file occurence mode (see ISO7816-4)
             *
             * @param fileOccurrence the {@link FileOccurrence}
             * @return the builder instance
             */
            AidSelectorBuilder& fileOccurrence(
                const FileOccurrence& fileOccurrence);

            /**
             * Sets the file control mode (see ISO7816-4)
             *
             * @param fileControlInformation the {@link FileControlInformation}
             * @return the builder instance
             */
            AidSelectorBuilder& fileControlInformation(
                const FileControlInformation& fileControlInformation);

            /**
             * Build a new {@code AidSelector}.
             *
             * @return a new instance
             */
            std::unique_ptr<AidSelector> build();

            /**
             * /!\ C++ vs. Java: variable is private in JAva
             */
            std::vector<uint8_t> mAidToSelect;

            /**
             * /!\ C++ vs. Java: variable is private in JAva
             */
            FileOccurrence mFileOccurrence = FileOccurrence::FIRST;

            /**
             * /!\ C++ vs. Java: variable is private in JAva
             */
            FileControlInformation mFileControlInformation =
                FileControlInformation::FCI;
        private:
            /**
             * Private constructor
             */
            AidSelectorBuilder() {}
        };

        /**
         *
         */
        static const int AID_MIN_LENGTH = 5;

        /**
         *
         */
        static const int AID_MAX_LENGTH = 16;

        /**
         *
         */
        friend std::ostream& operator<<(std::ostream& os, const AidSelector& a);

        /**
         *
         */
        virtual ~AidSelector() {}

        /**
         * Gets a new builder.
         *
         * @return a new builder instance
         */
        static std::unique_ptr<AidSelectorBuilder> builder();

        /**
         * Getter for the AID provided at construction time
         *
         * @return byte array containing the AID
         */
        virtual const std::vector<uint8_t>& getAidToSelect() const;

        /**
         * @return the file occurrence parameter
         */
        virtual FileOccurrence getFileOccurrence() const;

        /**
         * @return the file control information parameter
         */
        virtual FileControlInformation getFileControlInformation() const;

        /**
         * Gets the list of successful selection status codes
         *
         * @return the list of status codes
         */
        virtual std::shared_ptr<std::set<int>>
            getSuccessfulSelectionStatusCodes() const;

        /**
         * Add as status code to be accepted to the list of successful selection
         * status codes
         *
         * @param statusCode the status code to be accepted
         */
        virtual void addSuccessfulStatusCode(int statusCode);

    private:
        /**
         *
         */
        const FileOccurrence mFileOccurrence;

        /**
         *
         */
        const FileControlInformation mFileControlInformation;

        /**
         *
         */
        const std::vector<uint8_t> mAidToSelect;

        /**
         * List of status codes in response to the select application command
         * that should be considered successful although they are different from
         * 9000
         */
        std::shared_ptr<std::set<int>> mSuccessfulSelectionStatusCodes =
            std::make_shared<std::set<int>>();

        /**
         * Private constructor
         */
        AidSelector(AidSelectorBuilder* builder);
    };

    /**
     * Static nested class to hold the data elements used to perform an ATR
     * based filtering
     */
    class KEYPLECORE_API AtrFilter final
    : public std::enable_shared_from_this<AtrFilter> {
    public:
        /**
         * Regular expression based filter
         *
         * @param atrRegex String hex regular expression
         */
        AtrFilter(const std::string& atrRegex);

        /**
         *
         */
        virtual ~AtrFilter(){};

        /**
         * Setter for the regular expression provided at construction time
         *
         * @param atrRegex expression string
         */
        virtual void setAtrRegex(const std::string& atrRegex);

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
         *
         */
        friend std::ostream& operator<<(std::ostream& os, const AtrFilter& af);

    private:
        /**
         * Regular expression dedicated to handle SE logical channel opening
         * based on ATR pattern
         */
        std::string mAtrRegex;
    };

    /**
     * Create a SeSelector to perform the SE selection<br>
     *
     * @since 0.9
     */
    class SeSelectorBuilder {
    public:
        /**
         * Friend declaration to give access to private members
         */
        friend SeSelector;

        /**
         * Sets the SE protocol
         *
         * @param seProtocol the {@link SeProtocol} of the targeted SE
         * @return the builder instance
         */
        SeSelectorBuilder& seProtocol(
            const std::shared_ptr<SeProtocol> seProtocol);

        /**
         * Sets the SE ATR Filter
         *
         * @param atrFilter the {@link AtrFilter} of the targeted SE
         * @return the builder instance
         */
        SeSelectorBuilder& atrFilter(
            const std::shared_ptr<AtrFilter> atrFilter);

        /**
         * Sets the SE AID Selector
         *
         * @param aidSelector the {@link AidSelector} of the targeted SE
         * @return the builder instance
         */
        SeSelectorBuilder& aidSelector(
            const std::shared_ptr<AidSelector> aidSelector);

        /**
         * Build a new {@code SeSelector}.
         *
         * @return a new instance
         */
        std::unique_ptr<SeSelector> build();

    protected :
        /**
         * Private constructor
         */
        SeSelectorBuilder() {}

    private:
        /**
         *
         */
        std::shared_ptr<SeProtocol> mSeProtocol;

        /**
         *
         */
        std::shared_ptr<AtrFilter> mAtrFilter;

        /**
         *
         */
        std::shared_ptr<AidSelector> mAidSelector;
    };

    /**
     * Gets a new builder.
     *
     * @return a new builder instance
     */
    static std::unique_ptr<SeSelectorBuilder> builder();

    /**
     *
     */
    virtual ~SeSelector() {}

    /**
     * Getter
     *
     * @return the {@link SeProtocol} provided at construction time
     */
    virtual const std::shared_ptr<SeProtocol> getSeProtocol() const;

    /**
     * Getter
     *
     * @return the {@link AtrFilter} provided at construction time
     */
    virtual std::shared_ptr<AtrFilter> getAtrFilter() const;

    /**
     * Getter
     *
     * @return the {@link AidSelector} provided at construction time
     */
    virtual std::shared_ptr<AidSelector> getAidSelector() const;

    /**
     * Gets the information string
     *
     * @return a string to be printed in logs
     */
    const std::string& getExtraInfo() const;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const SeSelector& ss);

    /**
     *
     */
     friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const std::shared_ptr<SeSelector>& ss);

protected:
    /**
     * Private constructor
     *
     * @param builder the SeSelector builder
     */
    SeSelector(SeSelectorBuilder* builder);

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(SeSelector));

    /**
     * Keep it as a pointer to allow derived classes overloaded functions
     */
    const std::shared_ptr<SeProtocol> mSeProtocol;

    /**
     *
     */
    const std::shared_ptr<AidSelector> mAidSelector;

    /**
     *
     */
    const std::shared_ptr<AtrFilter> mAtrFilter;
};

}
}
}
