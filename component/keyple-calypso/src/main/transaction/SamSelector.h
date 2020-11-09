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
#include <stdexcept>
#include <memory>

/* Core */
#include "SeSelector.h"

/* Calypso */
#include "SamIdentifier.h"
#include "SamRevision.h"

/* Common */
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy;

/**
 * The {@link SamSelector} class extends {@link SeSelector} to handle specific Calypso SAM needs
 * such as model identification.
 */
class KEYPLECALYPSO_API SamSelector final : public SeSelector {
public:
    /**
     * Builder of {@link SamSelector}
     *
     * @since 0.9
     */
    class KEYPLECALYPSO_API SamSelectorBuilder final : public SeSelector::SeSelectorBuilder {
    public:
        /**
         *
         */
        friend SamSelector;

        /**
         *
         */
        SamSelectorBuilder();

        /**
         * Sets the SAM revision
         *
         * @param samRevision the {@link SamRevision} of the targeted SAM
         * @return the builder instance
         */
        SamSelectorBuilder& samRevision(const SamRevision samRevision);

        /**
         * Sets the SAM serial number regex
         *
         * @param serialNumber the serial number of the targeted SAM as regex
         * @return the builder instance
         */
        SamSelectorBuilder& serialNumber(const std::string& serialNumber);

        /**
         * Sets the SAM identifier
         *
         * @param samIdentifier the {@link SamIdentifier} of the targeted SAM
         * @return the builder instance
         */
        SamSelectorBuilder& samIdentifier(
            const std::shared_ptr<SamIdentifier> samIdentifier);

        /**
         * Sets the unlock data
         *
         * @param unlockData a byte array containing the unlock data (8 or 16 bytes)
         * @return the builder instance
         * @throw IllegalArgumentException if the provided buffer size is not 8 or 16
         */
        SamSelectorBuilder& unlockData(const std::vector<uint8_t>& unlockData);

        /**
         * {@inheritDoc}
         */
        SamSelectorBuilder& seProtocol(
            const std::shared_ptr<SeProtocol> seProtocol) override;

        /**
         * {@inheritDoc}
         */
        SamSelectorBuilder& atrFilter(
            const std::shared_ptr<AtrFilter> atrFilter) override;

        /**
         * {@inheritDoc}
         */
        SamSelectorBuilder& aidSelector(
            const std::shared_ptr<AidSelector> aidSelector) override;

        /**
         * Build a new {@code SamSelector}.
         *
         * @return a new instance
         *
         * Return type should be
         *     std::shared_ptr<SamSelector>
         * ... but invalid covariant
         */
        std::shared_ptr<SeSelector> build() override;

    private:
        /**
         *
         */
        SamRevision mSamRevision = SamRevision::NO_REV;

        /**
         *
         */
        std::string mSerialNumber;

        /**
         *
         */
        std::vector<uint8_t> mUnlockData;
    };

    /**
     * Gets a new builder.
     *
     * @return a new builder instance
     */
    static std::shared_ptr<SamSelectorBuilder> builder();

    /**
     * Gets the targeted SAM revision
     *
     * @return the target SAM revision value
     */
    const SamRevision getTargetSamRevision() const;

    /**
     * Gets the SAM unlock data
     *
     * @return a byte array containing the unlock data or null if the unlock data is not set
     */
    const std::vector<uint8_t>& getUnlockData() const;

    /**
     * Private constructor
     *
     * C++ vs. Java: Should be private but would forbid usage of make_shared
     *               from SamSelectorBuilder class. Setting it
     *               public for now. Could use an intermediate derived class
     *               otherwise if need be.
     */
    SamSelector(SamSelectorBuilder* builder);

private:
    /**
     *
     */
    const SamRevision mTargetSamRevision;

    /**
     *
     */
    const std::vector<uint8_t> mUnlockData;
};

}
}
}
