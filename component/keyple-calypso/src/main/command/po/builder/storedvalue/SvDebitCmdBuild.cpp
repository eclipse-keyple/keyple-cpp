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

#include "SvDebitCmdBuild.h"

/* Calypso */
#include "SvDebitRespPars.h"

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

SvDebitCmdBuild::SvDebitCmdBuild(const PoClass& poClass,
                                 const PoRevision& poRevision,
                                 const int amount,
                                 const uint8_t kvc,
                                 const std::vector<uint8_t> date,
                                 const std::vector<uint8_t> time)
: AbstractPoCommandBuilder<SvDebitRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::SV_DEBIT),
      nullptr),
  mPoClass(poClass),
  mPoRevision(poRevision)
{
    /* @see Calypso Layer ID 8.02 (200108) */
    if (amount < 0 || amount > 32767)
        throw IllegalArgumentException("Amount is outside allowed boundaries (0 <= amount <= " \
                                       "32767)");

    if (date.empty() || time.empty())
        throw IllegalArgumentException("date and time cannot be null");

    if (date.size() != 2 || time.size() != 2)
        throw IllegalArgumentException("date and time must be 2-byte arrays");

    /*
     * Handle the dataIn size with signatureHi length according to PO revision (3.2 rev have a
     * 10-byte signature)
     */
    mDataIn = std::vector<uint8_t>(15 + (poRevision == PoRevision::REV3_2 ? 10 : 5));

    /* dataIn[0] will be filled in at the finalization phase */
    uint16_t shortAmount = static_cast<uint16_t>(-amount);
    mDataIn[1] = ((shortAmount >> 8) & 0xFF);
    mDataIn[2] = (shortAmount & 0xFF);
    mDataIn[3] = date[0];
    mDataIn[4] = date[1];
    mDataIn[5] = time[0];
    mDataIn[6] = time[1];
    mDataIn[7] = kvc;
    // dataIn[8]..dataIn[8+7+sigLen] will be filled in at the finalization phase.
}

void SvDebitCmdBuild::finalizeBuilder(const std::vector<uint8_t> debitComplementaryData)
{
    if ((mPoRevision == PoRevision::REV3_2 && debitComplementaryData.size() != 20) ||
        (mPoRevision != PoRevision::REV3_2 && debitComplementaryData.size() != 15))
        throw IllegalArgumentException("Bad SV prepare load data length.");

    const uint8_t p1 = debitComplementaryData[4];
    const uint8_t p2 = debitComplementaryData[5];

    mDataIn[0] = debitComplementaryData[6];
    System::arraycopy(debitComplementaryData, 0, mDataIn, 8, 4);
    System::arraycopy(debitComplementaryData, 7, mDataIn, 12, 3);
    System::arraycopy(debitComplementaryData, 10, mDataIn, 15, debitComplementaryData.size() - 10);

    mRequest = setApduRequest(mPoClass.getValue(), mCommand, p1, p2, mDataIn);
}

const std::vector<uint8_t> SvDebitCmdBuild::getSvDebitData() const
{
    std::vector<uint8_t> svDebitData(12);
    svDebitData[0] = mCommand.getInstructionByte();
    /*
     * svDebitData[1,2] / P1P2 not set because ignored
     * Lc is 5 bytes longer in revision 3.2
     */
    svDebitData[3] = mPoRevision == PoRevision::REV3_2 ? 0x19 : 0x14;
    /* Appends the fixed part of dataIn */
    System::arraycopy(mDataIn, 0, svDebitData, 4, 8);

    return svDebitData;
}

std::shared_ptr<SvDebitRespPars> SvDebitCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SvDebitRespPars>(apduResponse, this);
}

bool SvDebitCmdBuild::isSessionBufferUsed() const
{
    return false;
}

}
}
}
}
}
}
