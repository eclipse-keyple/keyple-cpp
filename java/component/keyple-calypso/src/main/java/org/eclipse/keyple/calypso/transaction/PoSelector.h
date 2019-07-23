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
#include "SeSelector.h"

/* Calypso */
#include "PoSelector.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using SeSelector    = org::eclipse::keyple::core::seproxy::SeSelector;
using SeProtocol    = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;

/**
    * The {@link PoSelector} class extends {@link SeSelector} to handle specific PO features such as
    * the additional successful status codes list (in response to a select application command)
    */
class PoSelector final : public SeSelector {
    /**
        * Indicates if an invalidated PO should be selected or not.
        * <p>
        * The acceptance of an invalid PO is determined with the additional successful status codes
        * specified in the {@link AidSelector}
        */
public:
    enum class InvalidatedPo {
        REJECT,
        ACCEPT
    };


                            /**
        * PoAidSelector embedding the Calypo PO additional successful codes list
        */
public:
    class PoAidSelector : public SeSelector::AidSelector {

    private:
        static const std::shared_ptr<std::set<int>> successfulSelectionStatusCodes;

    private:
        class HashSetAnonymousInnerClass : public std::unordered_set<int>, public std::enable_shared_from_this<HashSetAnonymousInnerClass> {
//            {
//                add(0x6283);
//            }
        };

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
    public:
        PoAidSelector(std::shared_ptr<IsoAid> aidToSelect, InvalidatedPo invalidatedPo, FileOccurrence fileOccurrence, FileControlInformation fileControlInformation);

        /**
            * Simplified constructor with default values for the FileOccurrence and
            * FileControlInformation (see {@link AidSelector})
            *
            * @param aidToSelect the application identifier
            * @param invalidatedPo an enum value to indicate if an invalidated PO should be accepted or
            *        not
            */
        PoAidSelector(std::shared_ptr<IsoAid> aidToSelect, InvalidatedPo invalidatedPo);

protected:
        std::shared_ptr<PoAidSelector> shared_from_this() {
            return std::static_pointer_cast<PoAidSelector>(AidSelector::shared_from_this());
        }
    };

    /**
        * PoAtrFilter to perform a PO selection based on its ATR
        * <p>
        * Could be completed to handle Calypso specific ATR filtering process.
        */
public:
    class PoAtrFilter : public SeSelector::AtrFilter {

        /**
            * Regular expression based filter
            *
            * @param atrRegex String hex regular expression
            */
    public:
        PoAtrFilter(const std::string &atrRegex);

protected:
        std::shared_ptr<PoAtrFilter> shared_from_this() {
            return std::static_pointer_cast<PoAtrFilter>(AtrFilter::shared_from_this());
        }
    };

public:
    /**
     * Create a PoSelector to perform the PO selection. See {@link SeSelector}
     *
     * @param seProtocol the SE communication protocol
     * @param poAtrFilter the ATR filter
     * @param poAidSelector the AID selection data
     * @param extraInfo information string (to be printed in logs)
     */
    PoSelector(SeProtocol& seProtocol, std::shared_ptr<PoAtrFilter> poAtrFilter, std::shared_ptr<PoAidSelector> poAidSelector,
               const std::string &extraInfo);

protected:
    std::shared_ptr<PoSelector> shared_from_this() {
        return std::static_pointer_cast<PoSelector>(SeSelector::shared_from_this());
    }
};

}
}
}
}
}
