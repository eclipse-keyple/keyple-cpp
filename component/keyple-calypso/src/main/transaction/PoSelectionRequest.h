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

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

/* Core */
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "PoClass.h"

/* Common */
#include "KeypleCalypsoExport.h"
#include "LoggerFactory.h"

/* Core */
#include "AbstractApduResponseParser.h"
#include "SeProtocol.h"

/* Calypso */
#include "PoSelector.h"
#include "ReadRecordsRespPars.h"
#include "SelectFileCmdBuild.h"
#include "SelectFileRespPars.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core;
using namespace keyple::core::command;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::parser;

/**
 * Specialized selection request to manage the specific characteristics of
 * Calypso POs
 */
class KEYPLECALYPSO_API PoSelectionRequest final
: public
  AbstractSeSelectionRequest<AbstractPoCommandBuilder<AbstractPoResponseParser>>
{
public:
    /**
     * Constructor.
     *
     * @param poSelector the selector to target a particular SE
     */
    PoSelectionRequest(std::shared_ptr<PoSelector> poSelector);

    /**
     * Read a single record from the indicated EF
     *
     * @param sfi the SFI of the EF to read
     * @param recordNumber the record number to read
     * @throw IllegalArgumentException if one of the provided argument is out of
     *        range
     */
    void prepareReadRecordFile(const uint8_t sfi, const int recordNumber);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     *
     * @param lid LID of the EF to select as a byte array
     * @throw IllegalArgumentException if the argument is not an array of 2
     *        bytes
     */
    void prepareSelectFile(const std::vector<uint8_t> lid);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     *
     * @param lid LID of the EF to select as a byte array
     * @throw IllegalArgumentException if the argument is not an array of 2
     *        bytes
     */
    void prepareSelectFile(const uint16_t lid);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     *
     * @param selectControl provides the navigation case: FIRST, NEXT or CURRENT
     */
    void prepareSelectFile(const SelectFileControl selectControl);

    /**
     * Create a CalypsoPo object containing the selection data received from the
     * plugin
     *
     * C++ vs. Java: function is protected in Java
     *
     * @param seResponse the SE response received
     * @return a {@link CalypsoPo}
     * @throw CalypsoDesynchronizedExchangesException if the number of responses
     *        is different from the number of requests
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     *
     * Return type should be
     *   std::shared_ptr<CalypdoPo>
     * ... but invalid covariant return type
     */
    const std::shared_ptr<AbstractMatchingSe> parse(
        std::shared_ptr<SeResponse> seResponse) override;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(PoSelectionRequest));

    /**
     *
     */
    const PoClass& mPoClass;
};

}
}
}
