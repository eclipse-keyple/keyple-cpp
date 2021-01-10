/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "CalypsoPoUtils.h"

/* Common */
#include "Arrays.h"
#include "IllegalStateException.h"
#include "KeypleStd.h"
#include "System.h"

/* Core */
#include "KeypleAssert.h"

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "CalypsoPoCommand.h"
#include "CalypsoPoPinException.h"
#include "PoClass.h"
#include "ReadRecordsRespPars.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::exception;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::common::exception;
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

const int CalypsoPoUtils::PIN_LENGTH = 4;

const uint8_t CalypsoPoUtils::STORED_VALUE_FILE_STRUCTURE_ID = 0x20;
const uint8_t CalypsoPoUtils::SV_RELOAD_LOG_FILE_SFI = 0x14;
const uint8_t CalypsoPoUtils::SV_RELOAD_LOG_FILE_NB_REC = 1;
const uint8_t CalypsoPoUtils::SV_DEBIT_LOG_FILE_SFI = 0x15;
const uint8_t CalypsoPoUtils::SV_DEBIT_LOG_FILE_NB_REC = 3;
const uint8_t CalypsoPoUtils::SV_LOG_FILE_REC_LENGTH = 29;

std::vector<uint8_t> CalypsoPoUtils::mPoChallenge;
uint8_t CalypsoPoUtils::mSvKvc;
std::vector<uint8_t> CalypsoPoUtils::mSvGetHeader;
std::vector<uint8_t> CalypsoPoUtils::mSvGetData;
std::vector<uint8_t> CalypsoPoUtils::mSvOperationSignature;

CalypsoPoUtils::CalypsoPoUtils() {}

std::shared_ptr<AbstractOpenSessionRespPars> CalypsoPoUtils::updateCalypsoPoOpenSession(
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

std::shared_ptr<CloseSessionRespPars> CalypsoPoUtils::updateCalypsoPoCloseSession(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<CloseSessionCmdBuild> closeSessionCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    (void)calypsoPo;

    std::shared_ptr<CloseSessionRespPars> closeSessionRespPars =
            closeSessionCmdBuild->createResponseParser(apduResponse);

    closeSessionRespPars->checkStatus();

    return closeSessionRespPars;
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

std::shared_ptr<UpdateRecordRespPars> CalypsoPoUtils::updateCalypsoPoUpdateRecord(
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

    calypsoPo->fillContent(writeRecordCmdBuild->getSfi(),
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

std::shared_ptr<PoGetChallengeRespPars> CalypsoPoUtils::updateCalypsoPoGetChallenge(
    std::shared_ptr<PoGetChallengeCmdBuild> poGetChallengeCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<PoGetChallengeRespPars> poGetChallengeRespPars =
            poGetChallengeCmdBuild->createResponseParser(apduResponse);

    poGetChallengeRespPars->checkStatus();

    mPoChallenge = apduResponse->getDataOut();

    return poGetChallengeRespPars;
}

std::shared_ptr<VerifyPinRespPars> CalypsoPoUtils::updateCalypsoVerifyPin(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<VerifyPinCmdBuild> verifyPinCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<VerifyPinRespPars> verifyPinRespPars =
        verifyPinCmdBuild->createResponseParser(apduResponse);

    calypsoPo->setPinAttemptRemaining(verifyPinRespPars->getRemainingAttemptCounter());

    try {
        verifyPinRespPars->checkStatus();
    } catch (const CalypsoPoCommandException& ex) {
        /*
         * Forward the exception if the operation do not target the reading of the attempt counter.
         * Catch it silently otherwise
         */
        (void)ex;
        if (!verifyPinCmdBuild->isReadCounterOnly())
            throw;
    }

    return verifyPinRespPars;
}

std::shared_ptr<SvGetRespPars> CalypsoPoUtils::updateCalypsoPoSvGet(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<SvGetCmdBuild> svGetCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    std::shared_ptr<SvGetRespPars> svGetRespPars =
        svGetCmdBuild->createResponseParser(apduResponse);

    svGetRespPars->checkStatus();

    calypsoPo->setSvData(svGetRespPars->getBalance(),
                         svGetRespPars->getTransactionNumber(),
                         svGetRespPars->getLoadLog(),
                         svGetRespPars->getDebitLog());

    mSvKvc = svGetRespPars->getCurrentKVC();
    mSvGetHeader = svGetRespPars->getSvGetCommandHeader();
    mSvGetData = svGetRespPars->getApduResponse()->getBytes();

    return svGetRespPars;
}

std::shared_ptr<AbstractPoResponseParser> CalypsoPoUtils::updateCalypsoPoSvOperation(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>> svOperationCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    (void)calypsoPo;

    std::shared_ptr<AbstractPoResponseParser> svOperationRespPars =
            svOperationCmdBuild->createResponseParser(apduResponse);

    svOperationRespPars->checkStatus();

    mSvOperationSignature = svOperationRespPars->getApduResponse()->getDataOut();

    return svOperationRespPars;
}

std::shared_ptr<AbstractPoResponseParser> CalypsoPoUtils::updateCalypsoInvalidateRehabilitate(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        invalidateRehabilitateCmdBuild,
    std::shared_ptr<ApduResponse> apduResponse)
{
    (void)calypsoPo;

    std::shared_ptr<AbstractPoResponseParser> invalidateRehabilitateRespPars =
            invalidateRehabilitateCmdBuild->createResponseParser(apduResponse);

    invalidateRehabilitateRespPars->checkStatus();

    return invalidateRehabilitateRespPars;
}

std::unique_ptr<DirectoryHeader> CalypsoPoUtils::createDirectoryHeader(
    const std::vector<uint8_t>& proprietaryInformation)
{
    std::vector<uint8_t> lAccessConditions(SEL_AC_LENGTH);
    System::arraycopy(proprietaryInformation, SEL_AC_OFFSET, lAccessConditions, 0, SEL_AC_LENGTH);

    std::vector<uint8_t> lKeyIndexes(SEL_NKEY_LENGTH);
    System::arraycopy(proprietaryInformation, SEL_NKEY_OFFSET, lKeyIndexes, 0, SEL_NKEY_LENGTH);

    const uint8_t lDfStatus = proprietaryInformation[SEL_DF_STATUS_OFFSET];

    const uint16_t lLid =
        (((proprietaryInformation[SEL_LID_OFFSET] << 8) & 0xff00) |
         (proprietaryInformation[SEL_LID_OFFSET + 1] & 0x00ff));

    return DirectoryHeader::builder()
            ->lid(lLid)
            .accessConditions(lAccessConditions)
            .keyIndexes(lKeyIndexes)
            .dfStatus(lDfStatus)
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
    const FileType fileType = getEfTypeFromPoValue(proprietaryInformation[SEL_EF_TYPE_OFFSET]);

    int lRecordSize;
    int lRecordsNumber;

    if (fileType == FileType::BINARY) {
        lRecordSize = ((proprietaryInformation[SEL_REC_SIZE_OFFSET] << 8) & 0x0000ff00) |
                       (proprietaryInformation[SEL_NUM_REC_OFFSET] & 0x000000ff);
        lRecordsNumber = 1;
    } else {
        lRecordSize = proprietaryInformation[SEL_REC_SIZE_OFFSET];
        lRecordsNumber = proprietaryInformation[SEL_NUM_REC_OFFSET];
    }

    std::vector<uint8_t> lAccessConditions(SEL_AC_LENGTH);
    System::arraycopy(proprietaryInformation, SEL_AC_OFFSET, lAccessConditions, 0, SEL_AC_LENGTH);

    std::vector<uint8_t> lKeyIndexes(SEL_NKEY_LENGTH);
    System::arraycopy(proprietaryInformation, SEL_NKEY_OFFSET, lKeyIndexes, 0, SEL_NKEY_LENGTH);

    const uint8_t lDfStatus = proprietaryInformation[SEL_DF_STATUS_OFFSET];

    const uint16_t lSharedReference =
        (((proprietaryInformation[SEL_DATA_REF_OFFSET] << 8) & 0xff00) |
         (proprietaryInformation[SEL_DATA_REF_OFFSET + 1] & 0x00ff));

    const uint16_t lLid =
        (((proprietaryInformation[SEL_LID_OFFSET] << 8) & 0xff00) |
         (proprietaryInformation[SEL_LID_OFFSET + 1] & 0x00ff));

    return FileHeader::builder()
               ->lid(lLid)
               .recordsNumber(lRecordsNumber)
               .recordSize(lRecordSize)
               .type(fileType)
               .accessConditions(lAccessConditions)
               .keyIndexes(lKeyIndexes)
               .dfStatus(lDfStatus)
               .sharedReference(lSharedReference)
               .build();
}

std::shared_ptr<AbstractPoResponseParser> CalypsoPoUtils::updateCalypsoPo(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>> commandBuilder,
    std::shared_ptr<ApduResponse> apduResponse)
{
    const uint8_t ins =
        commandBuilder->getCommandRef()->getInstructionByte();

    if (ins == CalypsoPoCommand::READ_RECORDS.getInstructionByte())
        return updateCalypsoPoReadRecords(
                   calypsoPo,
                   std::reinterpret_pointer_cast<ReadRecordsCmdBuild>(commandBuilder),
                   apduResponse);
    else if (ins == CalypsoPoCommand::SELECT_FILE.getInstructionByte())
        return updateCalypsoPoSelectFile(
                   calypsoPo,
                   std::reinterpret_pointer_cast<SelectFileCmdBuild>(commandBuilder),
                   apduResponse);
    else if (ins == CalypsoPoCommand::UPDATE_RECORD.getInstructionByte())
        return updateCalypsoPoUpdateRecord(
                   calypsoPo,
                   std::reinterpret_pointer_cast<UpdateRecordCmdBuild>(commandBuilder),
                   apduResponse);
    else if (ins == CalypsoPoCommand::WRITE_RECORD.getInstructionByte())
        return updateCalypsoPoWriteRecord(
                   calypsoPo,
                   std::reinterpret_pointer_cast<WriteRecordCmdBuild>(commandBuilder),
                   apduResponse);
    else if (ins == CalypsoPoCommand::APPEND_RECORD.getInstructionByte())
        return updateCalypsoPoAppendRecord(
                   calypsoPo,
                   std::reinterpret_pointer_cast<AppendRecordCmdBuild>(commandBuilder),
                   apduResponse);
    else if (ins == CalypsoPoCommand::DECREASE.getInstructionByte())
        return updateCalypsoPoDecrease(
                   calypsoPo,
                   std::reinterpret_pointer_cast<DecreaseCmdBuild>(commandBuilder),
                   apduResponse);
    else if (ins ==  CalypsoPoCommand::INCREASE.getInstructionByte())
        return updateCalypsoPoIncrease(
                 calypsoPo,
                 std::reinterpret_pointer_cast<IncreaseCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::OPEN_SESSION_10.getInstructionByte() ||
             ins == CalypsoPoCommand::OPEN_SESSION_24.getInstructionByte() ||
             ins == CalypsoPoCommand::OPEN_SESSION_31.getInstructionByte() ||
             ins == CalypsoPoCommand::OPEN_SESSION_32.getInstructionByte())
        return updateCalypsoPoOpenSession(
                 calypsoPo,
                 std::reinterpret_pointer_cast<
                     AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>(commandBuilder),
                 apduResponse);
     else if (ins == CalypsoPoCommand::CLOSE_SESSION.getInstructionByte())
        return updateCalypsoPoCloseSession(
                 calypsoPo,
                 std::reinterpret_pointer_cast<CloseSessionCmdBuild>(commandBuilder),
                 apduResponse);
    else if (ins == CalypsoPoCommand::GET_CHALLENGE.getInstructionByte())
        return updateCalypsoPoGetChallenge(
                    std::reinterpret_pointer_cast<PoGetChallengeCmdBuild>(commandBuilder),
                    apduResponse);
    else if (ins == CalypsoPoCommand::VERIFY_PIN.getInstructionByte())
        return updateCalypsoVerifyPin(
                   calypsoPo,
                   std::reinterpret_pointer_cast<VerifyPinCmdBuild>(commandBuilder),
                   apduResponse);
    else if (ins == CalypsoPoCommand::SV_GET.getInstructionByte())
        return updateCalypsoPoSvGet(
                   calypsoPo,
                   std::reinterpret_pointer_cast<SvGetCmdBuild>(commandBuilder),
                   apduResponse);
    else if (ins == CalypsoPoCommand::SV_RELOAD.getInstructionByte() ||
             ins == CalypsoPoCommand::SV_DEBIT.getInstructionByte() ||
             ins == CalypsoPoCommand::SV_UNDEBIT.getInstructionByte())
        return updateCalypsoPoSvOperation(
                   calypsoPo,
                   std::reinterpret_pointer_cast
                       <AbstractPoCommandBuilder<AbstractPoResponseParser>>(commandBuilder),
                   apduResponse);
    else if (ins == CalypsoPoCommand::INVALIDATE.getInstructionByte() ||
             ins == CalypsoPoCommand::REHABILITATE.getInstructionByte())
        return updateCalypsoInvalidateRehabilitate(
                   calypsoPo,
                   std::reinterpret_pointer_cast
                       <AbstractPoCommandBuilder<AbstractPoResponseParser>>(commandBuilder),
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
            ++responseIterator;
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

const std::vector<uint8_t>& CalypsoPoUtils::getPoChallenge()
{
    return mPoChallenge;
}

uint8_t CalypsoPoUtils::getSvKvc()
{
    return mSvKvc;
}

const std::vector<uint8_t>& CalypsoPoUtils::getSvGetHeader()
{
    return mSvGetHeader;
}

const std::vector<uint8_t> CalypsoPoUtils::getSvGetData()
{
    return mSvGetData;
}

const std::vector<uint8_t> CalypsoPoUtils::getSvOperationSignature()
{
    return mSvOperationSignature;
}

}
}
}