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
#include "Logger.h"
#include "LoggerFactory.h"

/* Calypso */
#include "AbstractPoResponseParser.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
using AbstractPoResponseParser   = org::eclipse::keyple::calypso::command::po::AbstractPoResponseParser;
using Logger                     = org::eclipse::keyple::common::Logger;
using LoggerFactory              = org::eclipse::keyple::common::LoggerFactory;

/**
    * This class provides status code properties and the getters to access to the structured fields of
    * data from response to a Select File command (available from the parent class).
    * <p>
    * The FCI structure is analyzed and all subfields are made available through as many getters.
    */
class EXPORT SelectFileRespPars final : public AbstractPoResponseParser {

private:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(SelectFileRespPars));
    static std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

private:
    class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
    public:
        StaticConstructor();
    };

private:
    static SelectFileRespPars::StaticConstructor staticConstructor;


    // File Type Values
public:
    static constexpr int FILE_TYPE_MF = 1;
    static constexpr int FILE_TYPE_DF = 2;
    static constexpr int FILE_TYPE_EF = 4;

    // EF Type Values
    static constexpr int EF_TYPE_DF = 0;
    static constexpr int EF_TYPE_BINARY = 1;
    static constexpr int EF_TYPE_LINEAR = 2;
    static constexpr int EF_TYPE_CYCLIC = 4;
    static constexpr int EF_TYPE_SIMULATED_COUNTERS = 8;
    static constexpr int EF_TYPE_COUNTERS = 9;

private:
    std::vector<char> fileBinaryData;

    int lid = 0;

    char sfi = 0;

    char fileType = 0;

    char efType = 0;

    int recSize = 0;

    char numRec = 0;

    std::vector<char> accessConditions;

    std::vector<char> keyIndexes;

    char simulatedCounterFileSfi = 0;

    char simulatedCounterNumber = 0;

    int sharedEf = 0;

    char dfStatus = 0;

    std::vector<char> rfu;

    std::vector<char> kvcInfo;

    std::vector<char> kifInfo;

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

    /**
        * Instantiates a new SelectFileRespPars.
        *
        */
public:
    SelectFileRespPars(std::shared_ptr<ApduResponse> response);

    bool isSelectionSuccessful();

    int getLid();

    char getSfi();

    char getFileType();

    char getEfType();

    int getRecSize();

    char getNumRec();

    std::vector<char> getAccessConditions();

    std::vector<char> getKeyIndexes();

    char getSimulatedCounterFileSfi();

    char getSimulatedCounterNumber();

    int getSharedEf();

    char getDfStatus();

    std::vector<char> getFileBinaryData();

    std::vector<char> getRfu();

    std::vector<char> getKvcInfo();

    std::vector<char> getKifInfo();

    std::vector<char> getSelectionData();

protected:
    std::shared_ptr<SelectFileRespPars> shared_from_this() {
        return std::static_pointer_cast<SelectFileRespPars>(AbstractPoResponseParser::shared_from_this());
    }
};

}
}
}
}
}
}
}
