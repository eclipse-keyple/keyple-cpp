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

#include "SamIdentifier.h"

/* Common */
#include "Pattern.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;

/* SAM IDENTIFIER ----------------------------------------------------------- */

SamIdentifier::SamIdentifier(const SamIdentifierBuilder* builder)
: mSamRevision(builder->mSamRevision),
  mSerialNumber(builder->mSerialNumber),
  mGroupReference(builder->mGroupReference) {}

std::unique_ptr<SamIdentifierBuilder> SamIdentifier::builder()
{
    return std::unique_ptr<SamIdentifierBuilder>(new SamIdentifierBuilder());
}

const SamRevision& SamIdentifier::getSamRevision() const
{
    return mSamRevision;
}

const std::string& SamIdentifier::getSerialNumber() const
{
    return mSerialNumber;
}

const std::string& SamIdentifier::getGroupReference() const
{
    return mGroupReference;
}

bool SamIdentifier::matches(const std::shared_ptr<SamIdentifier> samIdentifier)
    const
{
    if (samIdentifier == nullptr) {
        return true;
    }

    if (samIdentifier->getSamRevision() != SamRevision::AUTO &&
        samIdentifier->getSamRevision() != mSamRevision) {
        return false;
    }

    if (samIdentifier->getSerialNumber() != "") {
        Pattern* p = Pattern::compile(samIdentifier->getSerialNumber());
        if (!p->matcher(mSerialNumber)->matches())
            return false;
    }

    if (samIdentifier->getGroupReference() != "" &&
        samIdentifier->getGroupReference() != mGroupReference) {
        return false;
    }

    return true;
}

/* SAM IDENTIFIER BUILDER --------------------------------------------------- */

SamIdentifierBuilder& SamIdentifierBuilder::samRevision(
    const SamRevision samRevision)
{
    mSamRevision = samRevision;
    return *this;
}

SamIdentifierBuilder& SamIdentifierBuilder::serialNumber(
    const std::string& serialNumber)
{
    mSerialNumber = serialNumber;
    return *this;
}

SamIdentifierBuilder& SamIdentifierBuilder::groupReference(
    const std::string& groupReference)
{
    mGroupReference = groupReference;
    return *this;
}

std::unique_ptr<SamIdentifier> SamIdentifierBuilder::build()
{
    return std::unique_ptr<SamIndentifier>(new SamIdentifier(this));
}

}
}
}
