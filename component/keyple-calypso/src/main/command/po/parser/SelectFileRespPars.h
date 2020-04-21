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

#include <unordered_map>
#include <vector>
#include <memory>

/* Core */
#include "AbstractApduResponseParser_Import.h"

/* Common */
#include "Export.h"
#include "LoggerFactory.h"

/* Calypso */
#include "AbstractPoResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::core::command;
using namespace keyple::calypso::command::po;
using namespace keyple::common;

/**
 * This class provides status code properties and the getters to access to the structured fields of
 * data from response to a Select File command (available from the parent class).
 * <p>
 * The FCI structure is analyzed and all subfields are made available through as many getters.
 */
class EXPORT SelectFileRespPars final : public AbstractPoResponseParser {
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
     */
    SelectFileRespPars(std::shared_ptr<ApduResponse> response);

    /**
     *
     */
    bool isSelectionSuccessful();

    /**
     *
     */
    int getLid();

    /**
     *
     */
    uint8_t getSfi();

    /**
     *
     */
    uint8_t getFileType();

    /**
     *
     */
    uint8_t getEfType();

    /**
     *
     */
    int getRecSize();

    /**
     *
     */
    uint8_t getNumRec();

    /**
     *
     */
    std::vector<uint8_t> getAccessConditions();

    /**
     *
     */
    std::vector<uint8_t> getKeyIndexes();

    /**
     *
     */
    uint8_t getSimulatedCounterFileSfi();

    /**
     *
     */
    uint8_t getSimulatedCounterNumber();

    /**
     *
     */
    int getSharedEf();

    /**
     *
     */
    uint8_t getDfStatus();

    /**
     *
     */
    std::vector<uint8_t> getFileBinaryData();

    /**
     *
     */
    std::vector<uint8_t> getRfu();

    /**
     *
     */
    std::vector<uint8_t> getKvcInfo();

    /**
     *
     */
    std::vector<uint8_t> getKifInfo();

    /**
     *
     */
    std::vector<uint8_t> getSelectionData();

protected:
    /**
     *
     */
    std::shared_ptr<SelectFileRespPars> shared_from_this()
    {
        return std::static_pointer_cast<SelectFileRespPars>(
            AbstractPoResponseParser::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(SelectFileRespPars));

    /**
     *
     */
    static std::unordered_map<
        int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        STATUS_TABLE;

    /**
     *
     */
    class StaticConstructor
    : public std::enable_shared_from_this<StaticConstructor> {
    public:
        StaticConstructor();
    };

    /**
     *
     */
    static SelectFileRespPars::StaticConstructor staticConstructor;

    /**
     *
     */
    std::vector<uint8_t> fileBinaryData;

    /**
     *
     */
    int lid = 0;

    /**
     *
     */
    uint8_t sfi = 0;

    /**
     *
     */
    uint8_t fileType = 0;

    /**
     *
     */
    uint8_t efType = 0;

    /**
     *
     */
    int recSize = 0;

    /**
     *
     */
    uint8_t numRec = 0;

    /**
     *
     */
    std::vector<uint8_t> accessConditions;

    /**
     *
     */
    std::vector<uint8_t> keyIndexes;

    /**
     *
     */
    uint8_t simulatedCounterFileSfi = 0;

    /**
     *
     */
    uint8_t simulatedCounterNumber = 0;

    /**
     *
     */
    int sharedEf = 0;

    /**
     *
     */
    uint8_t dfStatus = 0;

    /**
     *
     */
    std::vector<uint8_t> rfu;

    /**
     *
     */
    std::vector<uint8_t> kvcInfo;

    /**
     *
     */
    std::vector<uint8_t> kifInfo;

    /**
     *
     */
    bool selectionSuccessful = false;

    /**
     * Method extracting the various fields from the FCI structure returned by the PO.
     * <p>
     * The successful flag (see isSelectionSuccessful) is based on the response status word.
     * <p>
     * The parsingDone flag is set to avoid multiple call to this method while getting several
     * attributes. TODO Handle Rev1/Rev2 PO
     */
    void parseResponse();
};

}
}
}
}
}
