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
#include "stringhelper.h"

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

/* FILE OCCURRENCE ----------------------------------------------------------- */

FileOccurrence FileOccurrence::FIRST("FIRST", InnerEnum::FIRST,
                                     static_cast<char>(0x00));
FileOccurrence FileOccurrence::LAST("LAST", InnerEnum::LAST,
                                    static_cast<char>(0x01));
FileOccurrence FileOccurrence::NEXT("NEXT", InnerEnum::NEXT,
                                    static_cast<char>(0x02));
FileOccurrence FileOccurrence::PREVIOUS("PREVIOUS", InnerEnum::PREVIOUS,
                                        static_cast<char>(0x03));

std::vector<FileOccurrence> FileOccurrence::mValueList;

FileOccurrence::StaticConstructor::StaticConstructor()
{
    mValueList.push_back(FIRST);
    mValueList.push_back(LAST);
    mValueList.push_back(NEXT);
    mValueList.push_back(PREVIOUS);
}

FileOccurrence::StaticConstructor FileOccurrence::mStaticConstructor;
int FileOccurrence::mNextOrdinal = 0;

FileOccurrence::FileOccurrence(const std::string& name, InnerEnum innerEnum,
                               char isoBitMask)
: mInnerEnumValue(innerEnum), mNameValue(name), mOrdinalValue(mNextOrdinal++),
  mIsoBitMask(isoBitMask) {}

FileOccurrence::FileOccurrence(const FileOccurrence& o)
: mInnerEnumValue(o.mInnerEnumValue), mNameValue(o.mNameValue),
  mOrdinalValue(o.mOrdinalValue), mIsoBitMask(o.mIsoBitMask) {}

char FileOccurrence::getIsoBitMask()
{
    return mIsoBitMask;
}

std::vector<FileOccurrence> FileOccurrence::values()
{
    return mValueList;
}

int FileOccurrence::ordinal()
{
    return mOrdinalValue;
}

FileOccurrence FileOccurrence::valueOf(const std::string& name)
{
    for (auto enumInstance : FileOccurrence::mValueList) {
        if (enumInstance.mNameValue == name) {
            return enumInstance;
        }
    }

    /* Make compiler happy */
    return FileOccurrence::mValueList.front();
}

bool FileOccurrence::operator==(const FileOccurrence& other) const
{
    return mOrdinalValue == other.mOrdinalValue;
}

bool FileOccurrence::operator!=(const FileOccurrence& other) const
{
    return this->mOrdinalValue != other.mOrdinalValue;
}

FileOccurrence& FileOccurrence::operator=(const FileOccurrence& other)
{
    this->mInnerEnumValue = other.mInnerEnumValue;
    this->mNameValue = other.mNameValue;
    this->mOrdinalValue = other.mOrdinalValue;
    this->mIsoBitMask = other.mIsoBitMask;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const FileOccurrence& fc)
{
    os << "FILEOCCURRENCE: {"
       << "NAMEVALUE = " << fc.mNameValue << ", "
       << "ORDINALVALUE = " << fc.mOrdinalValue << ", "
       << "ISOBITMASK = " << fc.mIsoBitMask
       << "}";

    return os;
}

/* FILE CONTROL INFORMATION ------------------------------------------------- */

FileControlInformation FileControlInformation::FCI("FCI", InnerEnum::FCI, 0x00);
FileControlInformation FileControlInformation::FCP("FCP", InnerEnum::FCP, 0x04);
FileControlInformation FileControlInformation::FMD("FMD", InnerEnum::FMD, 0x08);
FileControlInformation
    FileControlInformation::NO_RESPONSE("NO_RESPONSE", InnerEnum::NO_RESPONSE,
                                        0x0C);

std::vector<FileControlInformation> FileControlInformation::mValueList;

FileControlInformation::StaticConstructor::StaticConstructor()
{
    mValueList.push_back(FCI);
    mValueList.push_back(FCP);
    mValueList.push_back(FMD);
    mValueList.push_back(NO_RESPONSE);
}

FileControlInformation::StaticConstructor
    FileControlInformation::mStaticConstructor;
int FileControlInformation::mNextOrdinal = 0;

FileControlInformation::FileControlInformation(const std::string& name,
                                               InnerEnum innerEnum,
                                               char isoBitMask)
: mInnerEnumValue(innerEnum), mNameValue(name), mOrdinalValue(mNextOrdinal++),
  mIsoBitMask(isoBitMask) {}

char FileControlInformation::getIsoBitMask()
{
    return mIsoBitMask;
}

bool FileControlInformation::operator==(const FileControlInformation& other)
    const
{
    return mOrdinalValue == other.mOrdinalValue;
}

bool FileControlInformation::operator!=(const FileControlInformation& other)
    const
{
    return mOrdinalValue != other.mOrdinalValue;
}

std::vector<FileControlInformation> FileControlInformation::values()
{
    return mValueList;
}

int FileControlInformation::ordinal()
{
    return mOrdinalValue;
}

FileControlInformation FileControlInformation::valueOf(const std::string& name)
{
    for (auto enumInstance : FileControlInformation::mValueList) {
        if (enumInstance.mNameValue == name) {
            return enumInstance;
        }
    }

    /* Make compiler happy */
    return FileControlInformation::mValueList.front();
}

std::ostream& operator<<(std::ostream& os, const FileControlInformation& fci)
{
    os << "FILECONTROLINFORMATION: {"
       << "NAMEVALUE = " << fci.mNameValue << ", "
       << "ORDINALVALUE = " << fci.mOrdinalValue << ", "
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
            StringHelper::formatSimple(
                "Bad AID length: %d. The AID length should be between 5 and 15",
                 aid.size()));
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

std::unique_ptr<AidSelector> AidSelectorBuilder::build()
{
    return std::unique_ptr<AidSelector>(new AidSelector(this));
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

std::unique_ptr<SeSelector> SeSelectorBuilder::build()
{
    return std::unique_ptr<SeSelector>(new SeSelector(this));
}

/* SE SELECTOR ---------------------------------------------------------------*/

std::unique_ptr<SeSelectorBuilder> SeSelector::builder()
{
    return std::unique_ptr<SeSelectorBuilder>(new SeSelectorBuilder());
}

SeSelector::SeSelector(SeSelectorBuilder* builder)
: mSeProtocol(builder->mSeProtocol), mAidSelector(builder->mAidSelector),
  mAtrFilter(builder->mAtrFilter)
{
    if (logger)
        logger->trace("%", *this);
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
        os << "SEPROTOCOL = null";

    if (ss.mAidSelector)
        os << *(ss.mAidSelector.get()) << ", ";
    else
        os << "AIDSELECTOR = null, ";

    if (ss.mAtrFilter)
        os << *(ss.mAtrFilter.get()) << ", ";
    else
        os << "ATRFILTER = null, ";

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
