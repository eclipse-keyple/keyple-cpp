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

#include "AbstractSelectionLocalReader.h"
#include "AnswerToReset.h"
#include "ApduResponse.h"
#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "KeypleApplicationSelectionException.h"
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"
#include "SeRequest.h"
#include "SeSelector_Import.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::event;

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractSelectionLocalReader));

AbstractSelectionLocalReader::AbstractSelectionLocalReader(const std::string &pluginName, const std::string &readerName)
: AbstractLocalReader(pluginName, readerName)
{

}

std::shared_ptr<ApduResponse> AbstractSelectionLocalReader::openChannelForAid(std::shared_ptr<SeSelector::AidSelector> aidSelector)
{
    logger->debug("openLogicalChannel- \n");

    std::shared_ptr<ApduResponse> fciResponse;

    const std::vector<char> aid = aidSelector->getAidToSelect()->getValue();
    if (aid.empty()) {
        throw std::invalid_argument("AID must not be null for an AidSelector.");
    }

    logger->trace("[%s] openLogicalChannel => Select Application with AID = %s\n",
                  AbstractLoggedObservable<ReaderEvent>::getName(),
                  ByteArrayUtil::toHex(aid));

    /*
     * build a get response command the actual length expected by the SE in the get response
     * command is handled in transmitApdu
     */
    std::vector<char> selectApplicationCommand(6 + aid.size());
    selectApplicationCommand[0] = static_cast<char>(0x00); // CLA
    selectApplicationCommand[1] = static_cast<char>(0xA4); // INS
    selectApplicationCommand[2] = static_cast<char>(0x04); // P1: select by name
    // P2: b0,b1 define the File occurrence, b2,b3 define the File control information
    // we use the bitmask defined in the respective enums
    selectApplicationCommand[3] = static_cast<char>(aidSelector->getFileOccurrence().getIsoBitMask() |
                                                    aidSelector->getFileControlInformation().getIsoBitMask());
    selectApplicationCommand[4] = static_cast<char>(aid.size()); // Lc
    System::arraycopy(aid, 0, selectApplicationCommand, 5, aid.size()); // data
    selectApplicationCommand[5 + aid.size()] = static_cast<char>(0x00); // Le

    logger->trace("[%s] openLogicalChannel => final command = %s\n", AbstractLoggedObservable<ReaderEvent>::getName(),
                  ByteArrayUtil::toHex(selectApplicationCommand));

    /*
     * we use here processApduRequest to manage case 4 hack. The successful status codes list
     * for this command is provided.
     */
    fciResponse = processApduRequest(std::make_shared<ApduRequest>("Internal Select Application", selectApplicationCommand,
                                                                   true, aidSelector->getSuccessfulSelectionStatusCodes()));

    if (!fciResponse->isSuccessful()) {
        logger->trace("[%s] openLogicalChannel => Application Selection failed." \
                      "SELECTOR = %s\n", AbstractLoggedObservable<ReaderEvent>::getName(),
                      aidSelector);
    }

    return fciResponse;
}
}
}
}
}
