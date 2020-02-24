/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

/* Calypso */
#include "CalypsoSam.h"

/* Core */
#include "AbstractMatchingSe.h"
#include "SeResponse.h"
#include "ByteArrayUtil.h"
#include "SelectionStatus.h"

/* Common */
#include "Arrays.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "Matcher.h"
#include "Pattern.h"
#include "stringhelper.h"
#include "System.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::selection;
using namespace keyple::core::util;

CalypsoSam::CalypsoSam(std::shared_ptr<SeResponse> selectionResponse,
                       TransmissionMode transmissionMode,
                       const std::string& extraInfo)
: AbstractMatchingSe(selectionResponse, transmissionMode, extraInfo),
  samRevision(SamRevision::C1) /* Default value to please compiler */
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
        platform           = atrSubElements[0];
        applicationType    = atrSubElements[1];
        applicationSubType = atrSubElements[2];

        // determine SAM revision from Application Subtype
        switch (applicationSubType) {
        case static_cast<char>(0xC1):
            samRevision = SamRevision(SamRevision::C1);
            break;
        case static_cast<char>(0xD0):
        case static_cast<char>(0xD1):
        case static_cast<char>(0xD2):
            samRevision = SamRevision(SamRevision::S1D);
            break;
        case static_cast<char>(0xE1):
            samRevision = SamRevision(SamRevision::S1E);
            break;
        default:
            throw IllegalStateException(StringHelper::formatSimple(
                "Unknown SAM revision (unrecognized application "
                "subtype 0x%02X)",
                applicationSubType));
        }

        softwareIssuer   = atrSubElements[3];
        softwareVersion  = atrSubElements[4];
        softwareRevision = atrSubElements[5];
        System::arraycopy(atrSubElements, 6, serialNumber, 0, 4);
        if (logger->isTraceEnabled()) {
            logger->trace("%s",
                          StringHelper::formatSimple(
                              "SAM %s PLATFORM = %02X, APPTYPE = %02X, "
                              "APPSUBTYPE = %02X, SWISSUER =%02X, SWVERSION ="
                              " %02X, SWREVISION = %02X\n",
                              samRevision.getName().c_str(), platform,
                              applicationType, applicationSubType,
                              softwareIssuer, softwareVersion, softwareRevision)
                              .c_str());

            logger->trace("SAM SERIALNUMBER = %s\n",
                          ByteArrayUtil::toHex(serialNumber).c_str());
        }
    } else {
        throw IllegalStateException(StringHelper::formatSimple(
            "Unrecognized ATR structure: %s", atrString));
    }
}

SamRevision CalypsoSam::getSamRevision()
{
    return samRevision;
}

std::vector<char> CalypsoSam::getSerialNumber()
{
    return serialNumber;
}

char CalypsoSam::getPlatform()
{
    return platform;
}

char CalypsoSam::getApplicationType()
{
    return applicationType;
}

char CalypsoSam::getApplicationSubType()
{
    return applicationSubType;
}

char CalypsoSam::getSoftwareIssuer()
{
    return softwareIssuer;
}

char CalypsoSam::getSoftwareVersion()
{
    return softwareVersion;
}

char CalypsoSam::getSoftwareRevision()
{
    return softwareRevision;
}

}
}
}
