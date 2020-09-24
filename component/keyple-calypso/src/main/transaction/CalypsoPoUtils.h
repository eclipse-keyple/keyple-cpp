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

#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "AppendRecordRespPars.h"
#include "CalypsoPo.h"
#include "DecreaseRespPars.h"
#include "FileHeader.h"
#include "IncreaseRespPars.h"
#include "ReadRecordsCmdBuild.h"
#include "SelectFileCmdBuild.h"
#include "SelectFileControl.h"
#include "UpdateRecordRespPars.h"
#include "WriteRecordRespPars.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::command::po::parser::security;

using FileType = FileHeader::FileType;

/**
 * Utility class used to check Calypso specific data.
 */
class CalypsoPoUtils final {
public:
    /**
     *
     */
    static const int MASK_3_BITS;
    static const int MASK_4_BITS;
    static const int MASK_5_BITS;
    static const int MASK_7_BITS;
    static const int MASK_1_BYTE;
    static const int MASK_2_BYTES;
    static const int MASK_3_BYTES;

    /**
     * SFI
     */
    static const int SFI_MIN;
    static const int SFI_MAX;

    /**
     * Record number
     */
    static const int NB_REC_MIN;
    static const int NB_REC_MAX;

    /**
     * Counter number
     */
    static const int NB_CNT_MIN;
    static const int NB_CNT_MAX;

    /**
     * Counter value
     */
    static const int CNT_VALUE_MIN;
    static const int CNT_VALUE_MAX;

    /**
     * Le max
     */
    static const int LE_MAX;

    /**
     * File Type Values
     */
    static const int FILE_TYPE_MF;
    static const int FILE_TYPE_DF;
    static const int FILE_TYPE_EF;

    /**
     * EF Type Values
     */
    static const int EF_TYPE_DF;
    static const int EF_TYPE_BINARY;
    static const int EF_TYPE_LINEAR;
    static const int EF_TYPE_CYCLIC;
    static const int EF_TYPE_SIMULATED_COUNTERS;
    static const int EF_TYPE_COUNTERS;

    /**
     * Field offsets in select file response (tag/length excluded)
     */
    static const int SEL_SFI_OFFSET;
    static const int SEL_TYPE_OFFSET;
    static const int SEL_EF_TYPE_OFFSET;
    static const int SEL_REC_SIZE_OFFSET;
    static const int SEL_NUM_REC_OFFSET;
    static const int SEL_AC_OFFSET;
    static const int SEL_AC_LENGTH;
    static const int SEL_NKEY_OFFSET;
    static const int SEL_NKEY_LENGTH;
    static const int SEL_DF_STATUS_OFFSET;
    static const int SEL_KVCS_OFFSET;
    static const int SEL_KIFS_OFFSET;
    static const int SEL_DATA_REF_OFFSET;
    static const int SEL_LID_OFFSET;

public:
    /**
     * (package-private)<br>
     * Fills the CalypsoPo with the PO's response to a single command
     *
     * @param calypsoPo the {@link CalypsoPo} object to fill with the provided
     *        response from the PO
     * @param commandBuilder the builder of the command that get the response
     * @param apduResponse the APDU response returned by the PO to the command
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static std::shared_ptr<AbstractPoResponseParser> updateCalypsoPo(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
            commandBuilder,
        const std::shared_ptr<ApduResponse> apduResponse);

    /**
     * (package-private)<br>
     * Fills the CalypsoPo with the PO's responses to a list of commands
     *
     * @param calypsoPo the {@link CalypsoPo} object to fill with the provided
     *        response from the PO
     * @param commandBuilders the list of builders that get the responses
     * @param apduResponses the APDU responses returned by the PO to all commands
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static void updateCalypsoPo(
        std::shared_ptr<CalypsoPo> calypsoPo,
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
            AbstractPoResponseParser>>>& commandBuilders,
        const std::vector<std::shared_ptr<ApduResponse>>& apduResponses);

    /**
     * Create a Read Records command builder for the provided arguments
     *
     * @param poClass the class of the PO
     * @param sfi the SFI of the EF to read
     * @param recordNumber the record number to read
     * @return a {@link ReadRecordsCmdBuild} object
     */
    static std::unique_ptr<ReadRecordsCmdBuild> prepareReadRecordFile(
        const PoClass& poClass, const uint8_t sfi, const uint8_t recordNumber);

    /**
     * Create a Select File command builder for the provided LID
     *
     * @param poClass the class of the PO
     * @param lid the LID of the EF to select
     * @return a {@link SelectFileCmdBuild} object
     */
    static std::unique_ptr<SelectFileCmdBuild> prepareSelectFile(
        const PoClass& poClass, const std::vector<uint8_t>& lid);

    /**
     * Create a Select File command builder for the provided select control
     *
     * @param poClass the class of the PO
     * @param selectControl provides the navigation case: FIRST, NEXT or CURRENT
     * @return a {@link SelectFileCmdBuild} object
     */
    static std::unique_ptr<SelectFileCmdBuild> prepareSelectFile(
        const PoClass& poClass, const SelectFileControl selectControl);

private:
    /**
     * Private constructor
     */
    CalypsoPoUtils();

    /**
     * Updated the {@link CalypsoPo} object with the response to a Open Secure
     * Session command received from the PO <br>
     * The ratification status and the data read at the time of the session
     * opening are added to the CalypsoPo.
     *
     * @param calypsoPo the {@link CalypsoPo} object to update
     * @param openSessionCmdBuild the Open Secure Session command builder
     * @param apduResponse the response received
     * @return the created response parser
     */
    static std::shared_ptr<AbstractOpenSessionRespPars>
        updateCalypsoPoOpenSession(
            std::shared_ptr<CalypsoPo> calypsoPo,
            std::shared_ptr<AbstractOpenSessionCmdBuild<
                AbstractOpenSessionRespPars>> openSessionCmdBuild,
            const std::shared_ptr<ApduResponse> apduResponse);

    /**
     * Updated the {@link CalypsoPo} object with the response to a Read Record
     * command received from the PO <br>
     * The records read are added to the {@link CalypsoPo} file structure
     *
     * @param calypsoPo the {@link CalypsoPo} object to update
     * @param readRecordsCmdBuild the Read Records command builder
     * @param apduResponse the response received
     * @return the created response parser
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static std::shared_ptr<ReadRecordsRespPars> updateCalypsoPoReadRecords(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<ReadRecordsCmdBuild> readRecordsCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse);

    /**
     * Updated the {@link CalypsoPo} object with the response to a Select File
     * command received from the PO <br>
     * Depending on the content of the response, either a {@link FileHeader} is
     * added or the {@link DirectoryHeader} is updated
     *
     * @param calypsoPo the {@link CalypsoPo} object to update
     * @param selectFileCmdBuild the Select File command builder
     * @param apduResponse the response received
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static std::shared_ptr<SelectFileRespPars> updateCalypsoPoSelectFile(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<SelectFileCmdBuild> selectFileCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse);

    /**
     * Updated the {@link CalypsoPo} object with the response to a Update Record
     * command sent and received from the PO <br>
     * The records read are added to the {@link CalypsoPo} file structure
     *
     * @param calypsoPo the {@link CalypsoPo} object to update
     * @param updateRecordCmdBuild the Update Record command builder
     * @param apduResponse the response received
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static std::shared_ptr<UpdateRecordRespPars> updateCalypsoPoUpdateRecord(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<UpdateRecordCmdBuild> updateRecordCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse);

    /**
     * Updated the {@link CalypsoPo} object with the response to a Write Record
     * command sent and received from the PO <br>
     * The records read are added to the {@link CalypsoPo} file structure
     *
     * @param calypsoPo the {@link CalypsoPo} object to update
     * @param writeRecordCmdBuild the Write Record command builder
     * @param apduResponse the response received
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static std::shared_ptr<WriteRecordRespPars> updateCalypsoPoWriteRecord(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<WriteRecordCmdBuild> writeRecordCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse);

    /**
     * Updated the {@link CalypsoPo} object with the response to a Read Records
     * command received from the PO <br>
     * The records read are added to the {@link CalypsoPo} file structure
     *
     * @param appendRecordCmdBuild the Append Records command builder
     * @param calypsoPo the {@link CalypsoPo} object to update
     * @param apduResponse the response received
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static std::shared_ptr<AppendRecordRespPars> updateCalypsoPoAppendRecord(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<AppendRecordCmdBuild> appendRecordCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse);

    /**
     * Updated the {@link CalypsoPo} object with the response to a Decrease
     * command received from the PO <br>
     * The counter value is updated in the {@link CalypsoPo} file structure
     *
     * @param decreaseCmdBuild the Decrease command builder
     * @param calypsoPo the {@link CalypsoPo} object to update
     * @param apduResponse the response received
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static std::shared_ptr<DecreaseRespPars> updateCalypsoPoDecrease(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<DecreaseCmdBuild> decreaseCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse);

    /**
     * Updated the {@link CalypsoPo} object with the response to an Increase
     * command received from the PO <br>
     * The counter value is updated in the {@link CalypsoPo} file structure
     *
     * @param increaseCmdBuild the Increase command builder
     * @param calypsoPo the {@link CalypsoPo} object to update
     * @param apduResponse the response received
     * @throw CalypsoPoCommandException if a response from the PO was unexpected
     */
    static std::shared_ptr<IncreaseRespPars> updateCalypsoPoIncrease(
        std::shared_ptr<CalypsoPo> calypsoPo,
        std::shared_ptr<IncreaseCmdBuild> increaseCmdBuild,
        std::shared_ptr<ApduResponse> apduResponse);


    /**
     * Parses the proprietaryInformation field of a file identified as an DF and
     * create a {@link DirectoryHeader}
     *
     * @param proprietaryInformation from the response to a Select File command
     * @return a {@link DirectoryHeader} object
     */
    static std::unique_ptr<DirectoryHeader> createDirectoryHeader(
        const std::vector<uint8_t>& proprietaryInformation);

    /**
     * Converts the EF type value from the PO into a {@link
     * FileHeader::FileType} enum
     *
     * @param efType the value returned by the PO
     * @return the corresponding {@link FileHeader.FileType}
     */
    static FileType getEfTypeFromPoValue(const uint8_t efType);

    /**
     * Parses the proprietaryInformation field of a file identified as an EF and
     * create a {@link FileHeader}
     *
     * @param proprietaryInformation from the response to a Select File command
     * @return a {@link FileHeader} object
     */
    static std::unique_ptr<FileHeader> createFileHeader(
        const std::vector<uint8_t>& proprietaryInformation);
};

}
}
}
