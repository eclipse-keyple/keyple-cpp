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
#include <unordered_set>
#include <vector>
#include <memory>

/* Core */
#include "SeSelector_Import.h"

/* Common */
#include "Export.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;

/**
 * The {@link PoSelector} class extends {@link SeSelector} to handle specific PO features such as
 * the additional successful status codes list (in response to a select application command)
  */
class EXPORT PoSelector final : public SeSelector {
public: /**
     * Indicates if an invalidated PO should be selected or not.
     * <p>
     * The acceptance of an invalid PO is determined with the additional successful status codes
     * specified in the {@link AidSelector}
     */
    enum class InvalidatedPo { REJECT, ACCEPT };

    /**
     * PoAidSelector embedding the Calypo PO additional successful codes list
     */
    class EXPORT PoAidSelector : public SeSelector::AidSelector {
    public:
        /**
         * Create a {@link PoAidSelector} to select a Calypso PO with an AID through a select
         * application command.
         *
         * @param aidToSelect the application identifier
         * @param invalidatedPo an enum value to indicate if an invalidated PO should be accepted or
         *        not
         * @param fileOccurrence the ISO7816-4 file occurrence parameter (see
         *        {@link FileOccurrence})
         * @param fileControlInformation the ISO7816-4 file control information parameter (see
         *        {@link FileControlInformation})
         */
        PoAidSelector(std::shared_ptr<IsoAid> aidToSelect,
                      InvalidatedPo invalidatedPo,
                      FileOccurrence fileOccurrence,
                      FileControlInformation fileControlInformation);

        /**
         * Simplified constructor with default values for the FileOccurrence and
         * FileControlInformation (see {@link AidSelector})
         *
         * @param aidToSelect the application identifier
         * @param invalidatedPo an enum value to indicate if an invalidated PO should be accepted or
         *        not
         */
        PoAidSelector(std::shared_ptr<IsoAid> aidToSelect,
                      InvalidatedPo invalidatedPo);

    protected:
        /**
         *
         */
        std::shared_ptr<PoAidSelector> shared_from_this()
        {
            return std::static_pointer_cast<PoAidSelector>(
                AidSelector::shared_from_this());
        }

    private:
        /**
         *
         */
        static const std::shared_ptr<std::set<int>>
            successfulSelectionStatusCodes;

        /**
         *
         */
        class HashSetAnonymousInnerClass
        : public std::unordered_set<int>,
          public std::enable_shared_from_this<HashSetAnonymousInnerClass> {
            //            {
            //                add(0x6283);
            //            }
        };
    };

    /**
     * PoAtrFilter to perform a PO selection based on its ATR
     * <p>
     * Could be completed to handle Calypso specific ATR filtering process.
     */
    class EXPORT PoAtrFilter : public SeSelector::AtrFilter {
    public:
        /**
         * Regular expression based filter
         *
         * @param atrRegex String hex regular expression
         */
        PoAtrFilter(const std::string& atrRegex);

    protected:
        /**
         *
         */
        std::shared_ptr<PoAtrFilter> shared_from_this()
        {
            return std::static_pointer_cast<PoAtrFilter>(
                AtrFilter::shared_from_this());
        }
    };

    /**
     * Create a PoSelector to perform the PO selection. See {@link SeSelector}
     *
     * @param seProtocol the SE communication protocol
     * @param poAtrFilter the ATR filter
     * @param poAidSelector the AID selection data
     * @param extraInfo information string (to be printed in logs)
     */
    PoSelector(SeProtocol& seProtocol, std::shared_ptr<PoAtrFilter> poAtrFilter,
               std::shared_ptr<PoAidSelector> poAidSelector,
               const std::string& extraInfo);

protected:
    /**
     *
     */
    std::shared_ptr<PoSelector> shared_from_this()
    {
        return std::static_pointer_cast<PoSelector>(
            SeSelector::shared_from_this());
    }
};

}
}
}
