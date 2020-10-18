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

#include "SeSelector.h"

/* Common */
#include "IllegalArgumentException.h"
#include "KeypleStd.h"

/* Core */
#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "SeRequest.h"


namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

using AidSelectorBuilder     = SeSelector::AidSelector::AidSelectorBuilder;
using FileOccurrence         = SeSelector::AidSelector::FileOccurrence;
using FileControlInformation = SeSelector::AidSelector::FileControlInformation;
using AidSelector            = SeSelector::AidSelector;
using AtrFilter              = SeSelector::AtrFilter;
using SeSelectorBuilder      = SeSelector::SeSelectorBuilder;

/* FILE OCCURRENCE ---------------------------------------------------------- */

const FileOccurrence FileOccurrence::FIRST(0x00);
const FileOccurrence FileOccurrence::LAST(0x01);
const FileOccurrence FileOccurrence::NEXT(0x02);
const FileOccurrence FileOccurrence::PREVIOUS(0x03);

FileOccurrence::FileOccurrence(const uint8_t isoBitMask)
: mIsoBitMask(isoBitMask) {}

FileOccurrence::FileOccurrence(const FileOccurrence& o)
: mIsoBitMask(o.mIsoBitMask) {}

uint8_t FileOccurrence::getIsoBitMask() const
{
    return mIsoBitMask;
}

bool FileOccurrence::operator==(const FileOccurrence& o) const
{
    return mIsoBitMask == o.mIsoBitMask;
}

bool FileOccurrence::operator!=(const FileOccurrence& o) const
{
    return !(*this == o);
}

FileOccurrence& FileOccurrence::operator=(const FileOccurrence& o)
{
    mIsoBitMask = o.mIsoBitMask;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const FileOccurrence& fc)
{
    os << "FILEOCCURRENCE: {"
       << "ISOBITMASK = " << fc.mIsoBitMask
       << "}";

    return os;
}

/* FILE CONTROL INFORMATION ------------------------------------------------- */

const FileControlInformation FileControlInformation::FCI(0x00);
const FileControlInformation FileControlInformation::FCP(0x04);
const FileControlInformation FileControlInformation::FMD(0x08);
const FileControlInformation FileControlInformation::NO_RESPONSE(0x0C);

FileControlInformation::FileControlInformation(const uint8_t isoBitMask)
: mIsoBitMask(isoBitMask) {}


FileControlInformation::FileControlInformation(const FileControlInformation& o)
: mIsoBitMask(o.mIsoBitMask) {}

uint8_t FileControlInformation::getIsoBitMask() const
{
    return mIsoBitMask;
}

bool FileControlInformation::operator==(const FileControlInformation& o)
    const
{
    return mIsoBitMask == o.mIsoBitMask;
}

bool FileControlInformation::operator!=(const FileControlInformation& o)
    const
{
    return !(*this == o);
}

FileControlInformation& FileControlInformation::operator=(
    const FileControlInformation& o)
{
    mIsoBitMask = o.mIsoBitMask;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const FileControlInformation& fci)
{
    os << "FILECONTROLINFORMATION: {"
       << "ISOBITMASK = " << fci.mIsoBitMask
       << "}";

    return os;
}

/* AID SELECTOR --------------------------------------------------------------*/

AidSelector::AidSelector(AidSelectorBuilder* builder)
: mFileOccurrence(builder->mFileOccurrence),
  mFileControlInformation(builder->mFileControlInformation),
  mAidToSelect(builder->mAidToSelect),
  mSuccessfulSelectionStatusCodes(nullptr) {}

const std::vector<uint8_t>& AidSelector::getAidToSelect() const
{
    return mAidToSelect;
}

FileOccurrence AidSelector::getFileOccurrence() const
{
    return mFileOccurrence;
}

FileControlInformation AidSelector::getFileControlInformation() const
{
    return mFileControlInformation;
}

std::shared_ptr<std::set<int>> AidSelector::getSuccessfulSelectionStatusCodes()
    const
{
    return mSuccessfulSelectionStatusCodes;
}

void AidSelector::addSuccessfulStatusCode(int statusCode)
{
    /* The list is kept null until a code is added */
    if (!mSuccessfulSelectionStatusCodes)
        mSuccessfulSelectionStatusCodes = std::make_shared<std::set<int>>();

    mSuccessfulSelectionStatusCodes->insert(statusCode);
}

std::ostream& operator<<(std::ostream& os, const AidSelector& a)
{
    os << "AIDSELECTOR: {"
       << "AIDTOSELECT = " << a.mAidToSelect
       << a.mFileOccurrence
       << a.mFileControlInformation
       << a.mSuccessfulSelectionStatusCodes
       << "}";

    return os;
}

/* ATR FILTER ----------------------------------------------------------------*/

AtrFilter::AtrFilter(const std::string& atrRegex)
{
    mAtrRegex = atrRegex;
}

void AtrFilter::setAtrRegex(const std::string& atrRegex)
{
    mAtrRegex = atrRegex;
}

std::string AtrFilter::getAtrRegex()
{
    return mAtrRegex;
}

bool AtrFilter::atrMatches(const std::vector<uint8_t>& atr)
{
    bool m;

    if (mAtrRegex.length() != 0) {
        Pattern* p            = Pattern::compile(mAtrRegex);
        std::string atrString = ByteArrayUtil::toHex(atr);
        m                     = p->matcher(atrString)->matches();
    } else {
        m = true;
    }

    return m;
}

std::ostream& operator<<(std::ostream& os, const AtrFilter& af)
{
    os << "ATRFILTER: {"
       << "REGEX = " << af.mAtrRegex
       << "}";

    return os;
}

/* AID SELECTOR BUILDER ------------------------------------------------------*/

AidSelectorBuilder& AidSelectorBuilder::aidToSelect(
    const std::vector<uint8_t>& aid)
{
    if (aid.size() < AID_MIN_LENGTH || aid.size() > AID_MAX_LENGTH) {
        throw IllegalArgumentException(
                "Bad AID length: " + std::to_string(aid.size()) + ". " +
                "The AID length should be between 5 and 15");
    } else {
        mAidToSelect = aid;
    }

    return *this;
}

AidSelectorBuilder& AidSelectorBuilder::aidToSelect(const std::string& aid)
{
    return aidToSelect(ByteArrayUtil::fromHex(aid));
}

AidSelectorBuilder& AidSelectorBuilder::fileOccurrence(
    const FileOccurrence& fileOccurrence)
{
    mFileOccurrence = fileOccurrence;

    return *this;
}

AidSelectorBuilder& AidSelectorBuilder::fileControlInformation(
    const FileControlInformation& fileControlInformation)
{
    mFileControlInformation = fileControlInformation;

    return *this;
}

std::shared_ptr<AidSelector> AidSelectorBuilder::build()
{
    return std::make_shared<AidSelector>(this);
}


std::unique_ptr<AidSelectorBuilder> AidSelector::builder()
{
    return std::unique_ptr<AidSelectorBuilder>(new AidSelectorBuilder());
}

/* SE SELECTOR BUILDER -------------------------------------------------------*/

SeSelectorBuilder& SeSelectorBuilder::seProtocol(
    const std::shared_ptr<SeProtocol> seProtocol)
{
    mSeProtocol = seProtocol;

    return *this;
}

SeSelectorBuilder& SeSelectorBuilder::atrFilter(
    const std::shared_ptr<AtrFilter> atrFilter)
{
    mAtrFilter = atrFilter;

    return *this;
}

SeSelectorBuilder& SeSelectorBuilder::aidSelector(
    const std::shared_ptr<AidSelector> aidSelector)
{
    mAidSelector = aidSelector;

    return *this;
}

std::shared_ptr<SeSelector> SeSelectorBuilder::build()
{
    return std::make_shared<SeSelector>(this);
}

/* SE SELECTOR ---------------------------------------------------------------*/

std::shared_ptr<SeSelectorBuilder> SeSelector::builder()
{
    SeSelectorBuilder S();

    return std::make_shared<SeSelectorBuilder>();
}

SeSelector::SeSelector(SeSelectorBuilder* builder)
: mSeProtocol(builder->mSeProtocol),
  mAidSelector(builder->mAidSelector),
  mAtrFilter(builder->mAtrFilter)
{
    mLogger->trace("%", *this);
}

const std::shared_ptr<SeProtocol> SeSelector::getSeProtocol() const
{
    return mSeProtocol;
}

std::shared_ptr<AtrFilter> SeSelector::getAtrFilter() const
{
    return mAtrFilter;
}

std::shared_ptr<AidSelector> SeSelector::getAidSelector() const
{
    return mAidSelector;
}

std::ostream& operator<<(std::ostream& os, const SeSelector& ss)
{
    os << "SESELECTOR: {";

    if (ss.mSeProtocol)
        os << ss.mSeProtocol;
    else
        os << "SEPROTOCOL = null, ";

    if (ss.mAidSelector)
        os << *(ss.mAidSelector.get()) << ", ";
    else
        os << "AIDSELECTOR = null, ";

    if (ss.mAtrFilter)
        os << *(ss.mAtrFilter.get());
    else
        os << "ATRFILTER = null";

    os << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<SeSelector>& ss)
{
	if (ss)
		os << *(ss.get());
    else
		os << "SESELECTOR = null";

    return os;
}

}
}
}
