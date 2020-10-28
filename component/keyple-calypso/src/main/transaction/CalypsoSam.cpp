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

/* Calypso */
#include "CalypsoSam.h"

/* Core */
#include "AbstractMatchingSe.h"
#include "SeResponse.h"
#include "ByteArrayUtil.h"
#include "SelectionStatus.h"

/* Common */
#include "Arrays.h"
#include "IllegalStateException.h"
#include "KeypleStd.h"
#include "LoggerFactory.h"
#include "Matcher.h"
#include "Pattern.h"
#include "stringhelper.h"
#include "System.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::selection;
using namespace keyple::core::util;

CalypsoSam::CalypsoSam(std::shared_ptr<SeResponse> selectionResponse,
                       const TransmissionMode& transmissionMode)
: AbstractMatchingSe(selectionResponse, transmissionMode),
  mSamRevision(SamRevision::C1), /* Default value to please compiler */
  mSerialNumber(4)
{
    std::string atrString = ByteArrayUtil::toHex(
        selectionResponse->getSelectionStatus()->getAtr()->getBytes());
    if (atrString.empty()) {
        throw IllegalStateException("ATR should not be empty.");
    }

    /* extract the historical bytes from T3 to T12 */
    std::string extractRegex = "3B(.{6}|.{10})805A(.{20})829000";
    Pattern* pattern         = Pattern::compile(extractRegex);
    Matcher* matcher         = pattern->matcher(atrString);
    if (matcher->find(0)) {
        std::vector<uint8_t> atrSubElements =
            ByteArrayUtil::fromHex(matcher->group(2));
        mPlatform           = atrSubElements[0];
        mApplicationType    = atrSubElements[1];
        mApplicationSubType = atrSubElements[2];

        // determine SAM revision from Application Subtype
        switch (mApplicationSubType) {
        case 0xC1:
            mSamRevision = SamRevision(SamRevision::C1);
            break;
        case 0xD0:
        case 0xD1:
        case 0xD2:
            mSamRevision = SamRevision(SamRevision::S1D);
            break;
        case 0xE1:
            mSamRevision = SamRevision(SamRevision::S1E);
            break;
        default:
            throw IllegalStateException(
                    "Unknown SAM revision (unrecognized application subtype " +
                    StringHelper::uint8ToHexString(mApplicationSubType));
        }

        mSoftwareIssuer   = atrSubElements[3];
        mSoftwareVersion  = atrSubElements[4];
        mSoftwareRevision = atrSubElements[5];
        System::arraycopy(atrSubElements, 6, mSerialNumber, 0, 4);
        mLogger->trace("SAM % PLATFORM = %, APPTYPE = %, APPSUBTYPE = %, " \
                      "SWISSUER = %, SWVERSION = %, SWREVISION = %\n",
                      mSamRevision.getName(),
                      mPlatform,
                      mApplicationType,
                      mApplicationSubType,
                      mSoftwareIssuer,
                      mSoftwareVersion,
                      mSoftwareRevision);

        mLogger->trace("SAM SERIALNUMBER = %\n", mSerialNumber);
    } else {
        throw IllegalStateException("Unrecognized ATR structure: " + atrString);
    }
}

const SamRevision& CalypsoSam::getSamRevision() const
{
    return mSamRevision;
}

const std::vector<uint8_t>& CalypsoSam::getSerialNumber() const
{
    return mSerialNumber;
}

uint8_t CalypsoSam::getPlatform() const
{
    return mPlatform;
}

uint8_t CalypsoSam::getApplicationType() const
{
    return mApplicationType;
}

uint8_t CalypsoSam::getApplicationSubType() const
{
    return mApplicationSubType;
}

uint8_t CalypsoSam::getSoftwareIssuer() const
{
    return mSoftwareIssuer;
}

uint8_t CalypsoSam::getSoftwareVersion() const
{
    return mSoftwareVersion;
}

uint8_t CalypsoSam::getSoftwareRevision() const
{
    return mSoftwareRevision;
}

}
}
}
