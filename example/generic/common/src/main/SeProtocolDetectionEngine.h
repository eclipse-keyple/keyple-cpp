/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
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
#include <iostream>
#include <memory>

/* Example */
#include "AbstractReaderObserverEngine.h"

/* Core */
#include "AbstractDefaultSelectionsRequest.h"
#include "AbstractDefaultSelectionsResponse.h"
#include "SeSelection.h"

/* Forward class declarations */
namespace keyple { namespace seproxy { class SeReader; } }

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy;
using namespace keyple::core::selection;

/**
 * This code demonstrates the multi-protocols capability of the Keyple SeProxy
 * <ul>
 * <li>instantiates a PC/SC plugin for a reader which name matches the regular expression provided
 * by poReaderName.</li>
 * <li>uses the observable mechanism to handle SE insertion/detection</li>
 * <li>expects SE with various protocols (technologies)</li>
 * <li>shows the identified protocol when a SE is detected</li>
 * <li>executes a simple Hoplink reading when a Hoplink SE is identified</li>
 * </ul>
 * The program spends most of its time waiting for a Enter key before exit. The actual SE processing
 * is mainly event driven through the observability.
 */
class SeProtocolDetectionEngine : public AbstractReaderObserverEngine {
public:
    /**
     *
     */
    SeProtocolDetectionEngine();

    /**
     *
     */
    virtual ~SeProtocolDetectionEngine() {}

    /**
     * Assign reader to the transaction engine
     */
    virtual void setReader(std::shared_ptr<SeReader> poReader);

    /**
     *
     */
    virtual std::shared_ptr<AbstractDefaultSelectionsRequest> prepareSeSelection();

    /**
     * This method is called when a SE is inserted (or presented to the reader's antenna). It
     * executes a {@link DefaultSelectionRequest} and processes the {@link SelectionResponse}
     * showing the APDUs exchanges
     */
    void processSeMatch(std::shared_ptr<AbstractDefaultSelectionsResponse> selectionResponse) override;

    /**
     *
     */
    void processSeInsertion() override;

    /**
     *
     */
    void processSeRemoval() override;

    /**
     *
     */
    void processUnexpectedSeRemoval() override;

protected:
    /**
     *
     */
    std::shared_ptr<SeProtocolDetectionEngine> shared_from_this()
    {
        return std::static_pointer_cast<SeProtocolDetectionEngine>(AbstractReaderObserverEngine::shared_from_this());
    }

private:
    /**
     *
     */
    std::shared_ptr<SeReader> poReader;

    /**
     *
     */
    std::shared_ptr<SeSelection> seSelection;
};

}
}
}
}
