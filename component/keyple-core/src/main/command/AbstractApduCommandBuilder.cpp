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

#include <iostream>

#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"
#include "CommandsTable.h"

namespace keyple {
namespace core {
namespace command {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::command;

AbstractApduCommandBuilder::AbstractApduCommandBuilder(
    CommandsTable& commandReference, std::shared_ptr<ApduRequest> request)
{
    this->name = commandReference.getName();
    this->request = request;

    // set APDU name for non null request
    if (request != nullptr) {
        this->request->setName(commandReference.getName());
    }
}

AbstractApduCommandBuilder::AbstractApduCommandBuilder(
    const std::string &name, std::shared_ptr<ApduRequest> request)
{
    this->name = name;
    this->request = request;

    // set APDU name for non null request
    if (request != nullptr) {
        this->request->setName(name);
    }
}

void AbstractApduCommandBuilder::addSubName(const std::string &subName)
{
    if (subName.length() != 0) {
        this->name = this->name + " - " + subName;
        if (request != nullptr) {
            this->request->setName(this->name);
        }
    }
}

std::string AbstractApduCommandBuilder::getName()
{
    return this->name;
}

std::shared_ptr<ApduRequest> AbstractApduCommandBuilder::getApduRequest()
{
    return request;
}

}
}
}
