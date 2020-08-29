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

#include <memory>
#include <vector>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommand.h"
#include "PoClass.h"
#include "SelectFileControl.h"
#include "SelectFileRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

/**
 * This class provides the dedicated constructor to build the Select File APDU
 * commands.
 *
 */
class KEYPLECALYPSO_API SelectFileCmdBuild final
: public AbstractPoCommandBuilder<SelectFileRespPars> {
public:
    /**
     * Instantiates a new SelectFileCmdBuild to select the first, next or
     * current file in the current DF.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param selectFileControl the selection mode control: FIRST, NEXT or
     *        CURRENT
     */
    SelectFileCmdBuild(PoClass poClass, SelectFileControl selectFileControl);

    /**
     * Instantiates a new SelectFileCmdBuild to select the first, next or
     * current file in the current DF.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param selectionPath the file identifier path
     */
    SelectFileCmdBuild(PoClass poClass,
                       const std::vector<uint8_t>& selectionPath);

    /**
     *
     */
    std::shared_ptr<SelectFileRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     * This command doesn't modify the contents of the PO and therefore doesn't
     * uses the session buffer.
     *
     * @return false
     */
    bool isSessionBufferUsed() const override;

    /**
     * The selection path can be null if the chosen constructor targets the
     * current EF
     *
     * @return the selection path or null
     */
    const std::vector<uint8_t>& getPath() const;

    /**
     * The file selection control can be null if the chosen constructor targets
     * an explicit path
     *
     * @return the select file control or null
     */
    SelectFileControl getSelectFileControl() const;

private:
    /**
     *
     */
    CalypsoPoCommand& command = CalypsoPoCommand::SELECT_FILE;

    /**
     * Construction arguments
     */
    const std::vector<uint8_t> mPath;
    const SelectFileControl mSelectFileControl;
};

/**
 *
 */
std::ostream& operator<<(std::ostream& os,
                         const SelectFileCmdBuild::SelectControl& sc);

}
}
}
}
}
