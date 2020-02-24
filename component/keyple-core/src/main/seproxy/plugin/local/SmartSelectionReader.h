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

/* Common */
#include "Export.h"

/* Core */
#include "SeReader.h"
#include "SeSelector_Import.h"
#include "KeypleApplicationSelectionException.h"
#include "KeypleChannelControlException.h"
#include "KeypleIOReaderException.h"
#include "ApduResponse.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

/**
 * Interface implemented by readers able to handle natively the SE selection
 * process (e.g. Android OMAPI readers).
 */
class EXPORT SmartSelectionReader : public SeReader {
public:
    /**
     * Opens a logical channel for the provided AID
     *
     * @param aidSelector the selection data
     * @return an ApduResponse containing the SE answer to selection
     * @throws KeypleIOReaderException if a communication error occurs
     * @throws KeypleChannelControlException if channel control error occurs
     * @throws KeypleApplicationSelectionException if selection error occurs
     */
    virtual std::shared_ptr<ApduResponse>
        openChannelForAid(SeSelector::AidSelector& aidSelector) = 0;
};

}
}
}
}
}
