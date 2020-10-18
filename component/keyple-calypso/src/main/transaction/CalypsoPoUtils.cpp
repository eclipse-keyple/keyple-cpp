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

#include "CalypsoPoUtils.h"

/* Common */
#include "Arrays.h"
#include "IllegalStateException.h"
#include "KeypleStd.h"
#include "System.h"

/* Core */
#include "KeypleAssert.h"

/* Calypso */
#include "CalypsoPoCommand.h"
#include "PoClass.h"
#include "ReadRecordsRespPars.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::core::util;

const int CalypsoPoUtils::MASK_3_BITS = 0x7;
const int CalypsoPoUtils::MASK_4_BITS = 0xF;
const int CalypsoPoUtils::MASK_5_BITS = 0x1F;
const int CalypsoPoUtils::MASK_7_BITS = 0x7F;
const int CalypsoPoUtils::MASK_1_BYTE = 0xFF;
const int CalypsoPoUtils::MASK_2_BYTES = 0xFFFF;
const int CalypsoPoUtils::MASK_3_BYTES = 0xFFFFFF;
const int CalypsoPoUtils::SFI_MIN = 0;
const int CalypsoPoUtils::SFI_MAX = MASK_5_BITS;
const int CalypsoPoUtils::NB_REC_MIN = 1;
const int CalypsoPoUtils::NB_REC_MAX = 255;
const int CalypsoPoUtils::NB_CNT_MIN = 1;
const int CalypsoPoUtils::NB_CNT_MAX = 255;
const int CalypsoPoUtils::CNT_VALUE_MIN = 0;
const int CalypsoPoUtils::CNT_VALUE_MAX = 16777215;
const int CalypsoPoUtils::LE_MAX = 255;
const int CalypsoPoUtils::FILE_TYPE_MF = 1;
const int CalypsoPoUtils::FILE_TYPE_DF = 2;
const int CalypsoPoUtils::FILE_TYPE_EF = 4;
const int CalypsoPoUtils::EF_TYPE_DF = 0;
const int CalypsoPoUtils::EF_TYPE_BINARY = 1;
const int CalypsoPoUtils::EF_TYPE_LINEAR = 2;
const int CalypsoPoUtils::EF_TYPE_CYCLIC = 4;
const int CalypsoPoUtils::EF_TYPE_SIMULATED_COUNTERS = 8;
const int CalypsoPoUtils::EF_TYPE_COUNTERS = 9;
const int CalypsoPoUtils::SEL_SFI_OFFSET = 0;
const int CalypsoPoUtils::SEL_TYPE_OFFSET = 1;
const int CalypsoPoUtils::SEL_EF_TYPE_OFFSET = 2;
const int CalypsoPoUtils::SEL_REC_SIZE_OFFSET = 3;
const int CalypsoPoUtils::SEL_NUM_REC_OFFSET = 4;
const int CalypsoPoUtils::SEL_AC_OFFSET = 5;
const int CalypsoPoUtils::SEL_AC_LENGTH = 4;
const int CalypsoPoUtils::SEL_NKEY_OFFSET = 9;
const int CalypsoPoUtils::SEL_NKEY_LENGTH = 4;
const int CalypsoPoUtils::SEL_DF_STATUS_OFFSET = 13;
const int CalypsoPoUtils::SEL_KVCS_OFFSET = 14;
const int CalypsoPoUtils::SEL_KIFS_OFFSET = 17;
const int CalypsoPoUtils::SEL_DATA_REF_OFFSET = 14;
const int CalypsoPoUtils::SEL_LID_OFFSET = 21;

CalypsoPoUtils::CalypsoPoUtils() {}

std::shared_ptr<AbstractOpenSessionRespPars>
    CalypsoPoUtils::updateCalypsoPoOpenSession(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<AbstractOpenSessionCmdBuild<
            AbstractOpenSessionRespPars>> openSessionCmdBuild,
        const std::shared_ptr<ApduResponse> apduResponse)
{
    /* Create parser */
    std::shared_ptr<AbstractOpenSessionRespPars> openSessionRespPars =
        openSessionCmdBuild->createResponseParser(apduResponse);

    calypsoPo->setDfRatified(openSessionRespPars->wasRatified());

    const std::vector<uint8_t>& recordDataRead =
        openSessionRespPars->getRecordDataRead();

    if (static_cast<int>(recordDataRead.size()) > 0)
        calypsoPo->setContent(openSessionCmdBuild->getSfi(),
                              openSessionCmdBuild->getRecordNumber(),
                              recordDataRead);

    return openSessionRespPars;
}

std::shared_ptr<ReadRecordsRespPars> CalypsoPoUtils::updateCalypsoPoReadRecords(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<ReadRecordsCmdBuild> readRecordsCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    /* Create parser */
    std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars =
        readRecordsCmdBuild->createResponseParser(apduResponse);

    readRecordsRespPars->checkStatus();

    /* Iterate over read records to fill the CalypsoPo */
    for (const auto& entry : readRecordsRespPars->getRecords())
        calypsoPo->setContent(readRecordsCmdBuild->getSfi(),
                              entry.first,
                              entry.second);

    return readRecordsRespPars;
}

std::shared_ptr<SelectFileRespPars> CalypsoPoUtils::updateCalypsoPoSelectFile(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<SelectFileCmdBuild> selectFileCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<SelectFileRespPars> selectFileRespPars =
        selectFileCmdBuild->createResponseParser(apduResponse);

    selectFileRespPars->checkStatus();

    const std::vector<uint8_t>& proprietaryInformation =
        selectFileRespPars->getProprietaryInformation();

    const uint8_t sfi = proprietaryInformation[SEL_SFI_OFFSET];
    const uint8_t fileType = proprietaryInformation[SEL_TYPE_OFFSET];

    switch (fileType) {
    case FILE_TYPE_MF:
    case FILE_TYPE_DF:
        {
        std::shared_ptr<DirectoryHeader> directoryHeader =
            createDirectoryHeader(proprietaryInformation);
        calypsoPo->setDirectoryHeader(directoryHeader);
        break;
        }
    case FILE_TYPE_EF:
        {
        std::shared_ptr<FileHeader> fileHeader =
            createFileHeader(proprietaryInformation);
        calypsoPo->setFileHeader(sfi, fileHeader);
        break;
        }
    default:
        throw IllegalStateException(
                  "Unknown file type: " +
                  StringHelper::uint8ToHexString(fileType));
    }

    return selectFileRespPars;
}

std::shared_ptr<UpdateRecordRespPars>
    CalypsoPoUtils::updateCalypsoPoUpdateRecord(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<UpdateRecordCmdBuild> updateRecordCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<UpdateRecordRespPars> updateRecordRespPars =
        updateRecordCmdBuild->createResponseParser(apduResponse);

    updateRecordRespPars->checkStatus();

    calypsoPo->setContent(updateRecordCmdBuild->getSfi(),
                          updateRecordCmdBuild->getRecordNumber(),
                          updateRecordCmdBuild->getData());

    return updateRecordRespPars;
}

std::shared_ptr<WriteRecordRespPars> CalypsoPoUtils::updateCalypsoPoWriteRecord(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<WriteRecordCmdBuild> writeRecordCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<WriteRecordRespPars> writeRecordRespPars =
        writeRecordCmdBuild->createResponseParser(apduResponse);

    writeRecordRespPars->checkStatus();

    /**
     * TODO we should add another method to Calypso to emulate the behavior of
     * Write Record
     */
    calypsoPo->setContent(writeRecordCmdBuild->getSfi(),
                          writeRecordCmdBuild->getRecordNumber(),
                          writeRecordCmdBuild->getData());

    return writeRecordRespPars;
}

std::shared_ptr<AppendRecordRespPars>
    CalypsoPoUtils::updateCalypsoPoAppendRecord(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<AppendRecordCmdBuild> appendRecordCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<AppendRecordRespPars> appendRecordRespPars =
        appendRecordCmdBuild->createResponseParser(apduResponse);

    appendRecordRespPars->checkStatus();

    calypsoPo->addCyclicContent(appendRecordCmdBuild->getSfi(),
                                appendRecordCmdBuild->getData());

    return appendRecordRespPars;
}

std::shared_ptr<DecreaseRespPars> CalypsoPoUtils::updateCalypsoPoDecrease(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<DecreaseCmdBuild> decreaseCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<DecreaseRespPars> decreaseRespPars =
        decreaseCmdBuild->createResponseParser(apduResponse);

    decreaseRespPars->checkStatus();

    calypsoPo->setContent(decreaseCmdBuild->getSfi(), 1,
                          apduResponse->getDataOut(),
                          3 * (decreaseCmdBuild->getCounterNumber() - 1));

    return decreaseRespPars;
}

std::shared_ptr<IncreaseRespPars> CalypsoPoUtils::updateCalypsoPoIncrease(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<IncreaseCmdBuild> increaseCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<IncreaseRespPars> increaseRespPars =
        increaseCmdBuild->createResponseParser(apduResponse);

    increaseRespPars->checkStatus();

    calypsoPo->setContent(increaseCmdBuild->getSfi(), 1,
                          apduResponse->getDataOut(),
                          3 * (increaseCmdBuild->getCounterNumber() - 1));

    return increaseRespPars;
}

std::unique_ptr<DirectoryHeader> CalypsoPoUtils::createDirectoryHeader(
    const std::vector<uint8_t>& proprietaryInformation)
{
    std::vector<uint8_t> accessConditions(SEL_AC_LENGTH);
    System::arraycopy(proprietaryInformation, SEL_AC_OFFSET, accessConditions,
                      0, SEL_AC_LENGTH);

    std::vector<uint8_t> keyIndexes(SEL_NKEY_LENGTH);
    System::arraycopy(proprietaryInformation, SEL_NKEY_OFFSET, keyIndexes, 0,
                      SEL_NKEY_LENGTH);

    const uint8_t dfStatus = proprietaryInformation[SEL_DF_STATUS_OFFSET];

    const uint16_t lid =
        (((proprietaryInformation[SEL_LID_OFFSET] << 8) & 0xff00) |
         (proprietaryInformation[SEL_LID_OFFSET + 1] & 0x00ff));

    return DirectoryHeader::builder()
            ->lid(lid)
            .accessConditions(accessConditions)
            .keyIndexes(keyIndexes)
            .dfStatus(dfStatus)
            .kvc(PoTransaction::SessionSetting::AccessLevel::SESSION_LVL_PERSO,
                 proprietaryInformation[SEL_KVCS_OFFSET])
            .kvc(PoTransaction::SessionSetting::AccessLevel::SESSION_LVL_LOAD,
                 proprietaryInformation[SEL_KVCS_OFFSET + 1])
            .kvc(PoTransaction::SessionSetting::AccessLevel::SESSION_LVL_DEBIT,
                 proprietaryInformation[SEL_KVCS_OFFSET + 2])
            .kif(PoTransaction::SessionSetting::AccessLevel::SESSION_LVL_PERSO,
                 proprietaryInformation[SEL_KIFS_OFFSET])
            .kif(PoTransaction::SessionSetting::AccessLevel::SESSION_LVL_LOAD,
                 proprietaryInformation[SEL_KIFS_OFFSET + 1])
            .kif(PoTransaction::SessionSetting::AccessLevel::SESSION_LVL_DEBIT,
                 proprietaryInformation[SEL_KIFS_OFFSET + 2])
            .build();
}

FileType CalypsoPoUtils::getEfTypeFromPoValue(const uint8_t efType)
{
    FileType fileType;

    switch (efType) {
    case EF_TYPE_BINARY:
        fileType = FileType::BINARY;
        break;
    case EF_TYPE_LINEAR:
        fileType = FileType::LINEAR;
        break;
    case EF_TYPE_CYCLIC:
        fileType = FileType::CYCLIC;
        break;
    case EF_TYPE_SIMULATED_COUNTERS:
        fileType = FileType::SIMULATED_COUNTERS;
        break;
    case EF_TYPE_COUNTERS:
        fileType = FileType::COUNTERS;
        break;
    default:
        throw IllegalStateException("Unknown EF Type: " +
                                    std::to_string(efType));
    }

    return fileType;
}

std::unique_ptr<FileHeader> CalypsoPoUtils::createFileHeader(
    const std::vector<uint8_t>& proprietaryInformation)
{
    const FileType fileType =
        getEfTypeFromPoValue(proprietaryInformation[SEL_EF_TYPE_OFFSET]);

    int recordSize;
    int recordsNumber;

    if (fileType == FileType::BINARY) {
        recordSize =
            ((proprietaryInformation[SEL_REC_SIZE_OFFSET] << 8) & 0x0000ff00) |
             (proprietaryInformation[SEL_NUM_REC_OFFSET] & 0x000000ff);
        recordsNumber = 1;
    } else {
        recordSize = proprietaryInformation[SEL_REC_SIZE_OFFSET];
        recordsNumber = proprietaryInformation[SEL_NUM_REC_OFFSET];
    }

    std::vector<uint8_t> accessConditions(SEL_AC_LENGTH);
    System::arraycopy(proprietaryInformation, SEL_AC_OFFSET, accessConditions,
                      0, SEL_AC_LENGTH);

    std::vector<uint8_t> keyIndexes(SEL_NKEY_LENGTH);
    System::arraycopy(proprietaryInformation, SEL_NKEY_OFFSET, keyIndexes, 0,
                      SEL_NKEY_LENGTH);

    const uint8_t dfStatus = proprietaryInformation[SEL_DF_STATUS_OFFSET];

    const uint16_t sharedReference =
        (((proprietaryInformation[SEL_DATA_REF_OFFSET] << 8) & 0xff00) |
         (proprietaryInformation[SEL_DATA_REF_OFFSET + 1] & 0x00ff));

    const uint16_t lid =
        (((proprietaryInformation[SEL_LID_OFFSET] << 8) & 0xff00) |
         (proprietaryInformation[SEL_LID_OFFSET + 1] & 0x00ff));

    return FileHeader::builder()
               ->lid(lid)
               .recordsNumber(recordsNumber)
               .recordSize(recordSize)
               .type(fileType)
               .accessConditions(accessConditions)
               .keyIndexes(keyIndexes)
               .dfStatus(dfStatus)
               .sharedReference(sharedReference)
               .build();
}

std::shared_ptr<AbstractPoResponseParser> CalypsoPoUtils::updateCalypsoPo(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        commandBuilder,
    std::shared_ptr<ApduResponse> apduResponse)
{
    const uint8_t ins =
        commandBuilder->getCommandRef()->getInstructionByte();

    if (ins == CalypsoPoCommand::READ_RECORDS.getInstructionByte())
        return updateCalypsoPoReadRecords(
                 calypsoPo,
                 reinterpret_pointer_cast<ReadRecordsCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::SELECT_FILE.getInstructionByte())
        return updateCalypsoPoSelectFile(
                 calypsoPo,
                 reinterpret_pointer_cast<SelectFileCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::UPDATE_RECORD.getInstructionByte())
        return updateCalypsoPoUpdateRecord(
                 calypsoPo,
                 reinterpret_pointer_cast<UpdateRecordCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::WRITE_RECORD.getInstructionByte())
        return updateCalypsoPoWriteRecord(
                 calypsoPo,
                 reinterpret_pointer_cast<WriteRecordCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::APPEND_RECORD.getInstructionByte())
        return updateCalypsoPoAppendRecord(
                 calypsoPo,
                 reinterpret_pointer_cast<AppendRecordCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::DECREASE.getInstructionByte())
        return updateCalypsoPoDecrease(
                 calypsoPo,
                 reinterpret_pointer_cast<DecreaseCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins ==  CalypsoPoCommand::INCREASE.getInstructionByte())
        return updateCalypsoPoIncrease(
                 calypsoPo,
                 reinterpret_pointer_cast<IncreaseCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::OPEN_SESSION_10.getInstructionByte() ||
             ins == CalypsoPoCommand::OPEN_SESSION_24.getInstructionByte() ||
             ins == CalypsoPoCommand::OPEN_SESSION_31.getInstructionByte() ||
             ins == CalypsoPoCommand::OPEN_SESSION_32.getInstructionByte())
        return updateCalypsoPoOpenSession(
                 calypsoPo,
                 reinterpret_pointer_cast<
                     AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>(
                         commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::CHANGE_KEY.getInstructionByte() ||
             ins == CalypsoPoCommand::GET_DATA_FCI.getInstructionByte() ||
             ins == CalypsoPoCommand::GET_DATA_TRACE.getInstructionByte())
        throw IllegalStateException("Shouldn't happen for now!");
    else
        throw IllegalStateException("Unknown command reference.");
}

void CalypsoPoUtils::updateCalypsoPo(
    std::shared_ptr<CalypsoPo> calypsoPo,
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>>& commandBuilders,
    const std::vector<std::shared_ptr<ApduResponse>>& apduResponses)
{
    std::vector<std::shared_ptr<ApduResponse>>::const_iterator responseIterator
        = apduResponses.begin();

    if (!commandBuilders.empty()) {
        for (const auto& commandBuilder : commandBuilders) {
            std::shared_ptr<ApduResponse> apduResponse = *responseIterator;
            updateCalypsoPo(calypsoPo, commandBuilder, apduResponse);
            responseIterator++;
        }
    }
}

std::unique_ptr<ReadRecordsCmdBuild> CalypsoPoUtils::prepareReadRecordFile(
    const PoClass& poClass, const uint8_t sfi, const uint8_t recordNumber)
{
    KeypleAssert::getInstance()
        .isInRange(sfi, CalypsoPoUtils::SFI_MIN, CalypsoPoUtils::SFI_MAX, "sfi")
        .isInRange(recordNumber,
                   CalypsoPoUtils::NB_REC_MIN,
                   CalypsoPoUtils::NB_REC_MAX,
                   "recordNumber");

    return std::unique_ptr<ReadRecordsCmdBuild>(
              new ReadRecordsCmdBuild(poClass,
                                      sfi,
                                      recordNumber,
                                      ReadRecordsCmdBuild::ReadMode::ONE_RECORD,
                                      0));
}

std::unique_ptr<SelectFileCmdBuild> CalypsoPoUtils::prepareSelectFile(
    const PoClass& poClass, const std::vector<uint8_t>& lid)
{
    KeypleAssert::getInstance()
        //.notNull(lid, "lid") /* Can't be null, it's a reference */
        .isEqual(lid.size(), 2, "lid");

    return std::unique_ptr<SelectFileCmdBuild>(
               new SelectFileCmdBuild(poClass, lid));
}

std::unique_ptr<SelectFileCmdBuild> CalypsoPoUtils::prepareSelectFile(
    const PoClass& poClass, const SelectFileControl selectControl)
{
    return std::unique_ptr<SelectFileCmdBuild>(
               new SelectFileCmdBuild(poClass, selectControl));
}

}
}
}