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
 * The {@link SamSelector} class extends {@link SeSelector} to handle specific
 * Calypso SAM needs such as model identification.
 */
class KEYPLECALYPSO_API SamSelector final : public SeSelector {
public:
    /**
     * Builder of {@link SamSelector}
     *
     * @since 0.9
     */
    class SamSelectorBuilder : public SeSelector::SeSelectorBuilder {
    public:
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
        SamSelectorBuilder& samIdentifier(const SamIdentifier& samIdentifier);

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
        SamRevision mSamRevision;

        /**
         *
         */
        std::string mSerialNumber;
    };

    /**
     * Gets a new builder.
     *
     * @return a new builder instance
     */
    static std::unique_ptr<SamSelectorBuilder> builder();

private:
    /**
     * Private constructor
     */
    SamSelector(SamSelectorBuilder* builder);
};

}
}
}
