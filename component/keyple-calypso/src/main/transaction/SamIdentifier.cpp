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

#include "SamIdentifier.h"

/* Common */
#include "Pattern.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;

SamIdentifier::SamIdentifier(
  const SamRevision& samRevision, const std::string& serialNumber,
  const std::string& groupReference)
: samRevision(samRevision), serialNumber(serialNumber),
  groupReference(groupReference)
{
}

const SamRevision& SamIdentifier::getSamRevision() const
{
    return samRevision;
}

const std::string& SamIdentifier::getSerialNumber() const
{
    return serialNumber;
}

const std::string& SamIdentifier::getGroupReference() const
{
    return groupReference;
}

bool SamIdentifier::matches(const SamIdentifier* samIdentifier) const
{
    if (samIdentifier == nullptr) {
        return true;
    }

    if (samIdentifier->getSamRevision() != SamRevision::AUTO &&
        samIdentifier->getSamRevision() != samRevision) {
        return false;
    }

    if (!samIdentifier->getSerialNumber().empty()) {
        Pattern* p = Pattern::compile(samIdentifier->getSerialNumber());
        if (!p->matcher(serialNumber)->matches()) {
            return false;
        }
    }

    if (!samIdentifier->getGroupReference().empty() &&
        !samIdentifier->getGroupReference().compare(groupReference)) {
        return false;
    }

    return true;
}

}
}
}
