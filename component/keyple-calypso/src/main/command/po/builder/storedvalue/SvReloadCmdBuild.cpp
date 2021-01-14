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

#include "SvReloadCmdBuild.h"

/* Calypso */
#include "SvReloadRespPars.h"

/* Common */
#include "IllegalArgumentException.h"
#include "System.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace storedvalue {

using namespace keyple::common;
using namespace keyple::common::exception;

SvReloadCmdBuild::SvReloadCmdBuild(const PoClass& poClass,
                                   const PoRevision& poRevision,
                                   const int amount,
                                   const uint8_t kvc,
                                   const std::vector<uint8_t>& date,
                                   const std::vector<uint8_t>& time,
                                   const std::vector<uint8_t>& free)
: AbstractPoCommandBuilder<SvReloadRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::SV_RELOAD),
      nullptr),
  mPoClass(poClass),
  mPoRevision(poRevision)
{
    if (amount < -8388608 || amount > 8388607)
        throw IllegalArgumentException("Amount is outside allowed boundaries (-8388608 <= amount " \
                                       "<=  8388607)");

    if (date.empty() || time.empty() || free.empty())
        throw IllegalArgumentException("date, time and free cannot be null");

    if (date.size() != 2 || time.size() != 2 || free.size() != 2)
        throw IllegalArgumentException("date, time and free must be 2-byte arrays");

    /*
     * Handle the mDataIn size with signatureHi length according to PO revision (3.2 rev have a
     * 10-byte signature)
     */
    mDataIn = std::vector<uint8_t>(18 + (poRevision == PoRevision::REV3_2 ? 10 : 5));

    /* mDataIn[0] will be filled in at the finalization phase */
    mDataIn[1] = date[0];
    mDataIn[2] = date[1];
    mDataIn[3] = free[0];
    mDataIn[4] = kvc;
    mDataIn[5] = free[1];
    mDataIn[6] = ((amount >> 16) & 0xFF);
    mDataIn[7] = ((amount >> 8) & 0xFF);
    mDataIn[8] = (amount & 0xFF);
    mDataIn[9] = time[0];
    mDataIn[10] = time[1];
    /* mDataIn[11]..mDataIn[11+7+sigLen] will be filled in at the finalization phase */
}

void SvReloadCmdBuild::finalizeBuilder(const std::vector<uint8_t>& reloadComplementaryData)
{
    if ((mPoRevision == PoRevision::REV3_2 && reloadComplementaryData.size() != 20) ||
        (mPoRevision != PoRevision::REV3_2 && reloadComplementaryData.size() != 15))
        throw IllegalArgumentException("Bad SV prepare load data length.");

    const uint8_t p1 = reloadComplementaryData[4];
    const uint8_t p2 = reloadComplementaryData[5];

    mDataIn[0] = reloadComplementaryData[6];
    System::arraycopy(reloadComplementaryData, 0, mDataIn, 11, 4);
    System::arraycopy(reloadComplementaryData, 7, mDataIn, 15, 3);
    System::arraycopy(reloadComplementaryData, 10, mDataIn, 18, reloadComplementaryData.size()-10);

    mRequest = setApduRequest(mPoClass.getValue(), mCommand, p1, p2, mDataIn);
}

const std::vector<uint8_t> SvReloadCmdBuild::getSvReloadData() const
{
    std::vector<uint8_t> svReloadData(15);
    svReloadData[0] = mCommand.getInstructionByte();
    /*
     * svReloadData[1,2] / P1P2 not set because ignored
     * Lc is 5 bytes longer in revision 3.2
     */
    svReloadData[3] = mPoRevision == PoRevision::REV3_2 ? 0x1C : 0x17;
    /* Appends the fixed part of dataIn */
    System::arraycopy(mDataIn, 0, svReloadData, 4, 11);

    return svReloadData;
}

std::shared_ptr<SvReloadRespPars> SvReloadCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SvReloadRespPars>(apduResponse, this);
}

bool SvReloadCmdBuild::isSessionBufferUsed() const
{
    return false;
}

}
}
}
}
}
}
