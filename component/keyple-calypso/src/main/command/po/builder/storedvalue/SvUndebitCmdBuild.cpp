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

#include "SvUndebitCmdBuild.h"

/* Calypso */
#include "SvUndebitRespPars.h"

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

SvUndebitCmdBuild::SvUndebitCmdBuild(const PoClass& poClass,
                                   const PoRevision& poRevision,
                                   const int amount,
                                   const uint8_t kvc,
                                   const std::vector<uint8_t>& date,
                                   const std::vector<uint8_t>& time)
: AbstractPoCommandBuilder<SvUndebitRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::SV_RELOAD),
      nullptr),
  mPoClass(poClass),
  mPoRevision(poRevision)
{
    /**
     * @see Calypso Layer ID 8.02 (200108)
     * @see Ticketing Layer Recommendations 170 (200108)
     */
    if (amount < 0 || amount > 32767)
        throw IllegalArgumentException("Amount is outside allowed boundaries (0 <= amount <= " \
                                       "32767)");

    if (date.empty() || time.empty())
        throw IllegalArgumentException("date and time cannot be null");

    if (date.size() != 2 || time.size() != 2)
        throw IllegalArgumentException("date and time must be 2-byte arrays");

    /*
     * Handle the mDataIn size with signatureHi length according to PO revision (3.2 rev have a
     * 10-byte signature)
     */
    mDataIn = std::vector<uint8_t>(15 + (poRevision == PoRevision::REV3_2 ? 10 : 5));

    /* mDataIn[0] will be filled in at the finalization phase */
    mDataIn[1] = ((amount >> 8) & 0xFF);
    mDataIn[2] = (amount & 0xFF);
    mDataIn[3] = date[0];
    mDataIn[4] = date[1];
    mDataIn[5] = time[0];
    mDataIn[6] = time[1];
    mDataIn[7] = kvc;
    /* mDataIn[8]..mDataIn[8+7+sigLen] will be filled in at the finalization phase */
}

void SvUndebitCmdBuild::finalizeBuilder(const std::vector<uint8_t>& undebitComplementaryData)
{
    if ((mPoRevision == PoRevision::REV3_2 && undebitComplementaryData.size() != 20) ||
        (mPoRevision != PoRevision::REV3_2 && undebitComplementaryData.size() != 15))
        throw IllegalArgumentException("Bad SV prepare load data length.");

    const uint8_t p1 = undebitComplementaryData[4];
    const uint8_t p2 = undebitComplementaryData[5];

    mDataIn[0] = undebitComplementaryData[6];
    System::arraycopy(undebitComplementaryData, 0, mDataIn, 8, 4);
    System::arraycopy(undebitComplementaryData, 7, mDataIn, 12, 3);
    System::arraycopy(undebitComplementaryData, 10, mDataIn, 15,
                      undebitComplementaryData.size() - 10);

    mRequest = setApduRequest(mPoClass.getValue(), mCommand, p1, p2, mDataIn);
}

const std::vector<uint8_t> SvUndebitCmdBuild::getSvUndebitData() const
{
    std::vector<uint8_t> svUndebitData(12);
    svUndebitData[0] = mCommand.getInstructionByte();
    /*
     * svUndebitData[1,2] / P1P2 not set because ignored
     * Lc is 5 bytes longer in revision 3.2
     */
    svUndebitData[3] = mPoRevision == PoRevision::REV3_2 ? 0x19 : 0x14;
    /* Appends the fixed part of mDataIn */
    System::arraycopy(mDataIn, 0, svUndebitData, 4, 8);

    return svUndebitData;
}

std::shared_ptr<SvUndebitRespPars> SvUndebitCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SvUndebitRespPars>(apduResponse, this);
}

bool SvUndebitCmdBuild::isSessionBufferUsed() const
{
    return false;
}

}
}
}
}
}
}
