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

/* Core */
#include "SeSelector.h"

/* Calypso */
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

/**
 * The {@link PoSelector} class extends {@link SeSelector} to handle specific PO
 * features such as the additional successful status codes list (in response to
 * a select application command)
 */
class KEYPLECALYPSO_API PoSelector final : public SeSelector {
public:
    /**
     * Indicates if an invalidated PO should be selected or not.
     * <p>
     * The acceptance of an invalid PO is determined with the additional
     * successful status codes specified in the {@link AidSelector}
     */
    enum class InvalidatedPo {
        REJECT,
        ACCEPT
    };

    /**
     * Builder of PoSelector
     *
     * @since 0.9
     */
    class KEYPLECALYPSO_API PoSelectorBuilder final : public SeSelector::SeSelectorBuilder {
    public:
        /**
         *
         */
        friend PoSelector;

        /**
         * Sets the desired behaviour in case of invalidated POs
         *
         * @param invalidatedPo the {@link InvalidatedPo} wanted behaviour
         * @return the builder instance
         */
        PoSelectorBuilder& invalidatedPo(const InvalidatedPo invalidatedPo);

        /**
         * {@inheritDoc}
         */
        PoSelectorBuilder& seProtocol(
            const std::shared_ptr<SeProtocol> seProtocol) override;

        /**
         * {@inheritDoc}
         */
        PoSelectorBuilder& atrFilter(const std::shared_ptr<AtrFilter> atrFilter)
            override;

        /**
         * {@inheritDoc}
         */
        PoSelectorBuilder& aidSelector(
            const std::shared_ptr<AidSelector> aidSelector) override;

        /**
         * Build a new {@code PoSelector}.
         *
         * @return a new instance
         *
         * Return type should be
         *   std::shared_ptr<PoSelector>
         * ... but invalid covariant return type
         */
        std::shared_ptr<SeSelector> build() override;

        /**
         * Private constructor
         *
         * C++ vs. Java: Should be private but would forbid usage of make_shared
         *               from PoSelectorBuilder class. Setting it public for
         *               now. Could use an intermediate derived class otherwise
         *               if need be.
         */
        PoSelectorBuilder();

    private:
        /**
         *
         */
        InvalidatedPo mInvalidatedPo;
    };

    /**
     * Gets a new builder.
     *
     * @return a new builder instance
     */
    static std::shared_ptr<PoSelectorBuilder> builder();

    /**
     * Private constructor
     *
     * C++ vs. Java: Should be private but would forbid usage of make_shared
     *               from PoSelectorBuilder class. Setting it public for now.
     *               Could use an intermediate derived class otherwise if need
     *               be.
     */
    PoSelector(PoSelectorBuilder* builder);

private:
    /**
     *
     */
    static const int SW_PO_INVALIDATED = 0x6283;
};

}
}
}
