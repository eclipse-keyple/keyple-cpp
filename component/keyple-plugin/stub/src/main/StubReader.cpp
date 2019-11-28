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

/* Core */
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"
#include "KeypleReaderException.h"
#include "SeProtocol_Import.h"
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "SeRequestSet.h"
#include "SeResponseSet.h"

/* Plugin - Stub */
#include "StubReader.h"
#include "StubSecureElement.h"

/* Common */
#include "Thread.h"
#include "InterruptedException.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::plugin;
using namespace keyple::core::seproxy::protocol;

const std::string StubReader::ALLOWED_PARAMETER_1   = "parameter1";
const std::string StubReader::ALLOWED_PARAMETER_2   = "parameter2";
const std::string StubReader::CONTACTLESS_PARAMETER = "contactless";
const std::string StubReader::CONTACTS_PARAMETER    = "contacts";
const std::string StubReader::pluginName            = "StubPlugin";

StubReader::StubReader(const std::string &name)
: AbstractThreadedLocalReader(pluginName, name)
{
    logger->debug("constructor (pluginName: %s, name: %s)\n",
                  pluginName.c_str(), name.c_str());

    readerName                   = name;
    sePresent                    = false;
    threadWaitTimeout            = 5000;
}

StubReader::~StubReader()
{
    logger->debug("destructor (name: %s)\n", name.c_str());
}

const std::vector<uint8_t>& StubReader::getATR()
{
    return se->getATR();
}

bool StubReader::isPhysicalChannelOpen()
{
    return se != nullptr && se->isPhysicalChannelOpen();
}

void StubReader::openPhysicalChannel()
{
    if (se != nullptr)
    {
        se->openPhysicalChannel();
    }
}

void StubReader::closePhysicalChannel()
{
    if (se != nullptr)
    {
        se->closePhysicalChannel();
    }
}

std::vector<char> StubReader::transmitApdu(std::vector<char> &apduIn)
{
    return se->processApdu(apduIn);
}

bool StubReader::protocolFlagMatches(const SeProtocol& protocolFlag)
{
    bool result;
    // Test protocolFlag to check if ATR based protocol filtering is required
    //if (protocolFlag != nullptr) {
        if (!isPhysicalChannelOpen()) {
            openPhysicalChannel();
        }

        /*
         * The requestSet will be executed only if the protocol match the
         * requestElement.
         */
        std::string selectionMask = protocolsMap[protocolFlag];
        if (selectionMask == "") {
            throw KeypleReaderException("Target selector mask not found!");
        }

        Pattern *p = Pattern::compile(selectionMask);
        std::string protocol       = se->getSeProcotol();
        if (!p->matcher(protocol)->matches()) {
            logger->trace("[%s] protocolFlagMatches => unmatching SE. " \
                          "PROTOCOLFLAG = %s\n", this->getName().c_str(),
                          protocolFlag.toString().c_str());
            result = false;
        }
        else
        {
            logger->trace("[%s] protocolFlagMatches => matching SE. " \
                          "PROTOCOLFLAG = %s\n", this->getName().c_str(),
                          protocolFlag.toString().c_str());
            result = true;
        }
    //}
    //else
    //{
    //    // no protocol defined returns true
    //    result = true;
    //}

    return result;
}

bool StubReader::checkSePresence()
{
    return se != nullptr;
}

void StubReader::setParameter(const std::string &name, const std::string &value)
{
    if (name == ALLOWED_PARAMETER_1 || name == ALLOWED_PARAMETER_2)
    {
        parameters.emplace(name, value);
    }
    else if (name == CONTACTS_PARAMETER)
    {
        transmissionMode = TransmissionMode::CONTACTS;
    }
    else if (name == CONTACTLESS_PARAMETER)
    {
        transmissionMode = TransmissionMode::CONTACTLESS;
    }
    else
    {
        throw std::make_shared<KeypleReaderException>(
                  "parameter name not supported : " + name);
    }
}

std::unordered_map<std::string, std::string> StubReader::getParameters()
{
    return parameters;
}

TransmissionMode StubReader::getTransmissionMode()
{
    return transmissionMode;
}

std::shared_ptr<ApduResponse> StubReader::processApduRequestTestProxy(
    std::shared_ptr<ApduRequest> apduRequest)
{
    return this->processApduRequest(apduRequest);
}

std::shared_ptr<SeResponseSet> StubReader::processSeRequestSetTestProxy(
    std::shared_ptr<SeRequestSet> requestSet)
{
    return this->processSeRequestSet(requestSet);
}

bool StubReader::isLogicalChannelOpenTestProxy()
{
    return this->isPhysicalChannelOpen();
}

void StubReader::insertSe(std::shared_ptr<StubSecureElement> _se)
{
    /* clean channels status */
    if (isPhysicalChannelOpen()) {
        logger->debug("closing logical channel\n");
        closeLogicalChannel();

        try {
            logger->debug("closing physical channel\n");
            closePhysicalChannel();
        } catch (KeypleReaderException &e) {
            logger->debug("insertSe - caught KeypleReaderException (msg: %s " \
                          ", cause: %s\n", e.getMessage().c_str(),
                          e.getCause().what());
        }
    }

    se        = _se;
    sePresent = true;
}

void StubReader::removeSe()
{
    logger->debug("removing SE\n");

    se = nullptr;
    sePresent = false;
}

bool StubReader::waitForCardPresent(long long timeout)
{
    logger->debug("waiting for card present (%d ms)\n", timeout);

    for (int i = 0; i < timeout / 10; i++)
    {
        if (sePresent)
        {
            logger->debug("SE present\n");
            break;
        }
        try
        {
            Thread::sleep(10);
        } catch (const InterruptedException &e)
        {
            logger->debug("Sleep was interrupted\n");
        }
    }

    return sePresent;
}

bool StubReader::waitForCardAbsent(long long timeout)
{
    for (int i = 0; i < timeout / 10; i++)
    {
        if (!sePresent)
        {
            break;
        }
        try
        {
            Thread::sleep(10);
        } catch (const InterruptedException &e)
        {
            logger->debug("Sleep was interrupted\n");
        }
    }
    return !sePresent;
}

bool StubReader::equals(std::shared_ptr<void> o)
{
    (void)o;

    return false;
}

int StubReader::hashCode()
{
    return 0;
}

void StubReader::setParameters(
    std::unordered_map<std::string, std::string> &parameters)
{
    (void)parameters;
}

}
}
}
