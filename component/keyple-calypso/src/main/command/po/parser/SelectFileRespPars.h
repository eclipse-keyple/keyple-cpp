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

#include <map>
#include <vector>
#include <memory>

/* Core */
#include "AbstractApduResponseParser.h"
#include "Tag.h"

/* Common */
#include "LoggerFactory.h"

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "KeypleCalypsoExport.h"
#include "SelectFileCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::core::command;
using namespace keyple::core::util::bertlv;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::common;

/**
 * This class provides status code properties and the getters to access to the
 * structured fields of data from response to a Select File command (available
 * from the parent class).
 * <p>
 * The value of the Proprietary Information tag is extracted from the Select
 * File response and made available using the corresponding getter.
 */
class KEYPLECALYPSO_API SelectFileRespPars final
: public AbstractPoResponseParser {
public:
    /**
     * File Type Values
     */
    static constexpr int FILE_TYPE_MF = 1;
    static constexpr int FILE_TYPE_DF = 2;
    static constexpr int FILE_TYPE_EF = 4;

    /**
     * EF Type Values
     */
    static constexpr int EF_TYPE_DF                 = 0;
    static constexpr int EF_TYPE_BINARY             = 1;
    static constexpr int EF_TYPE_LINEAR             = 2;
    static constexpr int EF_TYPE_CYCLIC             = 4;
    static constexpr int EF_TYPE_SIMULATED_COUNTERS = 8;
    static constexpr int EF_TYPE_COUNTERS           = 9;

    /**
     * Instantiates a new SelectFileRespPars.
     *
     * @param response the response from the PO
     * @param builder the reference to the builder that created this parser
     */
    SelectFileRespPars(std::shared_ptr<ApduResponse> response,
                       SelectFileCmdBuild* builder);

    /**
     * @return the content of the proprietary information tag present in the
     *         response to the Select File command
     */
    const std::vector<uint8_t>& getProprietaryInformation();

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(SelectFileRespPars));

    /**
     *
     */
    static const std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;

    /**
     *
     */
    std::vector<uint8_t> mProprietaryInformation;

    /**
     * Proprietary Information: context-specific class, primitive, tag number
     * 5h => tag field 85h
     */
    static const Tag TAG_PROPRIETARY_INFORMATION;
};

}
}
}
}
}
