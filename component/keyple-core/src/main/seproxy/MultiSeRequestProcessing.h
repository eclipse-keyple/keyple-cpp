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

namespace keyple {
namespace core {
namespace seproxy {

/**
 * indicates whether the selection process should stop after the first matching
 * or process all requests in the SeRequest Set
 */
enum class MultiSeRequestProcessing {
    /**
     * The selection process stops as soon as a selection case is successful.
     */
    FIRST_MATCH,
    /**
     * The selection process performs all the selection cases provided in the
     * Set of SeRequest.
     */
    PROCESS_ALL
};

}
}
}