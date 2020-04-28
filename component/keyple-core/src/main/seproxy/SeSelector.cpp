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
#include "SeSelector.h"
#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "SeRequest.h"

/* Common */
#include "stringhelper.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::core::seproxy;

using FileOccurrence         = SeSelector::AidSelector::FileOccurrence;
using FileControlInformation = SeSelector::AidSelector::FileControlInformation;
using IsoAid                 = SeSelector::AidSelector::IsoAid;
using AidSelector            = SeSelector::AidSelector;
using AtrFilter              = SeSelector::AtrFilter;

FileOccurrence FileOccurrence::FIRST("FIRST", InnerEnum::FIRST,
                                     static_cast<char>(0x00));
FileOccurrence FileOccurrence::LAST("LAST", InnerEnum::LAST,
                                    static_cast<char>(0x01));
FileOccurrence FileOccurrence::NEXT("NEXT", InnerEnum::NEXT,
                                    static_cast<char>(0x02));
FileOccurrence FileOccurrence::PREVIOUS("PREVIOUS", InnerEnum::PREVIOUS,
                                        static_cast<char>(0x03));

std::vector<FileOccurrence> FileOccurrence::valueList;

FileOccurrence::StaticConstructor::StaticConstructor()
{
    valueList.push_back(FIRST);
    valueList.push_back(LAST);
    valueList.push_back(NEXT);
    valueList.push_back(PREVIOUS);
}

FileOccurrence::StaticConstructor FileOccurrence::staticConstructor;
int FileOccurrence::nextOrdinal = 0;

FileOccurrence::FileOccurrence(const std::string& name, InnerEnum innerEnum,
                               char isoBitMask)
: innerEnumValue(innerEnum), nameValue(name), ordinalValue(nextOrdinal++)
{
    this->isoBitMask = isoBitMask;
}

FileOccurrence::FileOccurrence(const FileOccurrence& o)
: innerEnumValue(o.innerEnumValue), nameValue(o.nameValue),
  ordinalValue(o.ordinalValue), isoBitMask(o.isoBitMask)
{
}

char FileOccurrence::getIsoBitMask()
{
    return this->isoBitMask;
}

bool FileOccurrence::operator==(const FileOccurrence& other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool FileOccurrence::operator!=(const FileOccurrence& other)
{
    return this->ordinalValue != other.ordinalValue;
}

std::vector<FileOccurrence> FileOccurrence::values()
{
    return valueList;
}

int FileOccurrence::ordinal()
{
    return ordinalValue;
}

FileOccurrence FileOccurrence::valueOf(const std::string& name)
{
    for (auto enumInstance : FileOccurrence::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Make compiler happy */
    return FileOccurrence::valueList.front();
}

FileControlInformation FileControlInformation::FCI("FCI", InnerEnum::FCI, 0x00);
FileControlInformation FileControlInformation::FCP("FCP", InnerEnum::FCP, 0x04);
FileControlInformation FileControlInformation::FMD("FMD", InnerEnum::FMD, 0x08);
FileControlInformation
    FileControlInformation::NO_RESPONSE("NO_RESPONSE", InnerEnum::NO_RESPONSE,
                                        0x0C);

std::vector<FileControlInformation> FileControlInformation::valueList;

FileControlInformation::StaticConstructor::StaticConstructor()
{
    valueList.push_back(FCI);
    valueList.push_back(FCP);
    valueList.push_back(FMD);
    valueList.push_back(NO_RESPONSE);
}

FileControlInformation::StaticConstructor
    FileControlInformation::staticConstructor;
int FileControlInformation::nextOrdinal = 0;

FileControlInformation::FileControlInformation(const std::string& name,
                                               InnerEnum innerEnum,
                                               char isoBitMask)
: innerEnumValue(innerEnum), nameValue(name), ordinalValue(nextOrdinal++)
{
    this->isoBitMask = isoBitMask;
}

FileControlInformation::FileControlInformation(const FileControlInformation& o)
: innerEnumValue(o.innerEnumValue), nameValue(o.nameValue),
  ordinalValue(o.ordinalValue), isoBitMask(o.isoBitMask)
{
}

char FileControlInformation::getIsoBitMask()
{
    return this->isoBitMask;
}

bool FileControlInformation::operator==(const FileControlInformation& other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool FileControlInformation::operator!=(const FileControlInformation& other)
{
    return this->ordinalValue != other.ordinalValue;
}

std::vector<FileControlInformation> FileControlInformation::values()
{
    return valueList;
}

int FileControlInformation::ordinal()
{
    return ordinalValue;
}

FileControlInformation FileControlInformation::valueOf(const std::string& name)
{
    for (auto enumInstance : FileControlInformation::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Make compiler happy */
    return FileControlInformation::valueList.front();
}

SeSelector::AidSelector::IsoAid::IsoAid(const std::vector<uint8_t>& aid)
{
    if (aid.size() < AID_MIN_LENGTH || aid.size() > AID_MAX_LENGTH) {
        value.clear();
        throw std::invalid_argument(
            "Bad AID length: " + StringHelper::to_string(aid.size()) +
            ". The AID length should be " + "between 5 and 15.");
    } else {
        value = aid;
    }
}

IsoAid::IsoAid(const std::string& aid) : IsoAid(ByteArrayUtil::fromHex(aid))
{
}

const std::vector<uint8_t>& IsoAid::getValue()
{
    return value;
}

bool IsoAid::startsWith(std::shared_ptr<IsoAid> aid)
{
    if (this->value.size() > aid->getValue().size()) {
        return false;
    }

    for (int i = 0; i < (int)aid->getValue().size(); i++) {
        if (this->value[i] != aid->getValue()[i]) {
            return false;
        }
    }

    return true;
}

AidSelector::AidSelector(
    std::shared_ptr<IsoAid> aidToSelect,
    std::shared_ptr<std::set<int>> successfulSelectionStatusCodes,
    FileOccurrence fileOccurrence,
    FileControlInformation fileControlInformation)
: fileOccurrence(fileOccurrence), fileControlInformation(fileControlInformation)
{
    this->aidToSelect                    = aidToSelect;
    this->successfulSelectionStatusCodes = successfulSelectionStatusCodes;
}

AidSelector::AidSelector(
    std::shared_ptr<IsoAid> aidToSelect,
    std::shared_ptr<std::set<int>> successfulSelectionStatusCodes)
: AidSelector(aidToSelect, successfulSelectionStatusCodes,
              FileOccurrence::FIRST, FileControlInformation::FCI)
{
}

AidSelector::AidSelector(const AidSelector& o)
: std::enable_shared_from_this<AidSelector>(o),
  fileOccurrence(o.fileOccurrence),
  fileControlInformation(o.fileControlInformation),
  aidToSelect(o.aidToSelect),
  successfulSelectionStatusCodes(o.successfulSelectionStatusCodes)
{
}

AidSelector::~AidSelector()
{
}

std::shared_ptr<IsoAid> AidSelector::getAidToSelect()
{
    return aidToSelect;
}

FileOccurrence AidSelector::getFileOccurrence()
{
    return fileOccurrence;
}

FileControlInformation AidSelector::getFileControlInformation()
{
    return fileControlInformation;
}

std::shared_ptr<std::set<int>> AidSelector::getSuccessfulSelectionStatusCodes()
{
    return successfulSelectionStatusCodes;
}

SeSelector::AtrFilter::AtrFilter(const std::string& atrRegex)
{
    this->atrRegex = atrRegex;
}

void SeSelector::AtrFilter::setAtrRegex(const std::string& atrRegex)
{
    this->atrRegex = atrRegex;
}

std::string SeSelector::AtrFilter::getAtrRegex()
{
    return atrRegex;
}

bool SeSelector::AtrFilter::atrMatches(const std::vector<uint8_t>& atr)
{
    bool m;

    if (atrRegex.length() != 0) {
        Pattern* p            = Pattern::compile(atrRegex);
        std::string atrString = ByteArrayUtil::toHex(atr);
        m                     = p->matcher(atrString)->matches();
    } else {
        m = true;
    }

    return m;
}

SeSelector::SeSelector(SeProtocol& seProtocol,
                       std::shared_ptr<AtrFilter> atrFilter,
                       std::shared_ptr<AidSelector> aidSelector,
                       const std::string& extraInfo)
: seProtocol(seProtocol), aidSelector(aidSelector), atrFilter(atrFilter),
  extraInfo(extraInfo)
{
}

const SeProtocol& SeSelector::getSeProtocol() const
{
    return seProtocol;
}

std::shared_ptr<AtrFilter> SeSelector::getAtrFilter() const
{
    return atrFilter;
}

std::shared_ptr<AidSelector> SeSelector::getAidSelector() const
{
    return aidSelector;
}

const std::string& SeSelector::getExtraInfo() const
{
    return extraInfo;
}

std::ostream& operator<<(std::ostream& os,
                         const SeSelector::AidSelector::IsoAid& a)
{
	os << a.value;

    return os;
}

std::ostream& operator<<(std::ostream& os, const SeSelector::AtrFilter& af)
{
    os << "ATRFILTER: {REGEX: " << af.atrRegex << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os, const SeSelector::AidSelector& a)
{
    const std::shared_ptr<SeSelector::AidSelector::IsoAid>& aid = a.aidToSelect;

    os << "AID: ";
	if (aid)
		os << *(aid.get());
    else
		os << "null";

    return os;
}

std::ostream& operator<<(std::ostream& os, const SeSelector& ss)
{
    os << "SESELECTOR: {AID_SELECTOR = ";

    if (ss.aidSelector)
        os << *(ss.aidSelector.get());
    else
        os << "null";

    os << ", ATR_FILTER = ";

    if (ss.atrFilter)
        os << *(ss.atrFilter.get());
    else
		os << "null";
	
    os << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<SeSelector>& ss)
{
	if (ss)
		os << *(ss.get());
    else
		os << "SESELECTOR: null";

    return os;
}

}
}
}
