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

#include "ApduResponse.h"
#include "CalypsoPo.h"
#include "PoSelectionRequest.h"
#include "SeResponse.h"
#include "GetDataFciRespPars.h"
#include "ByteArrayUtil.h"
#include "SeResponse.h"
#include "SelectionStatus.h"

/* Common */
#include "IllegalStateException.h"
#include "NoSuchElementException.h"
#include "stringhelper.h"
#include "System.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::selection;
using namespace keyple::core::util;

const int CalypsoPo::SI_BUFFER_SIZE_INDICATOR = 0;
const int CalypsoPo::SI_PLATFORM = 1;
const int CalypsoPo::SI_APPLICATION_TYPE = 2;
const int CalypsoPo::SI_APPLICATION_SUBTYPE = 3;
const int CalypsoPo::SI_SOFTWARE_ISSUER = 4;
const int CalypsoPo::SI_SOFTWARE_VERSION = 5;
const int CalypsoPo::SI_SOFTWARE_REVISION = 6;

const uint8_t CalypsoPo::APP_TYPE_WITH_CALYPSO_PIN = 0x01;
const uint8_t CalypsoPo::APP_TYPE_WITH_CALYPSO_SV = 0x02;
const uint8_t CalypsoPo::APP_TYPE_RATIFICATION_COMMAND_REQUIRED = 0x04;
const uint8_t CalypsoPo::APP_TYPE_CALYPSO_REV_32_MODE = 0x08;
const uint8_t CalypsoPo::APP_TYPE_WITH_PUBLIC_AUTHENTICATION = 0x10;

const int CalypsoPo::PO_REV1_ATR_LENGTH = 20;
const int CalypsoPo::REV1_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION = 3;
const int CalypsoPo::REV2_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION = 6;

const std::vector<int> CalypsoPo::BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE = {
    0, 0, 0, 0, 0, 0, 215, 256, 304, 362, 430, 512, 608, 724, 861, 1024, 1217,
    1448, 1722, 2048, 2435, 2896, 3444, 4096, 4870, 5792, 6888, 8192, 9741,
    11585, 13777, 16384, 19483, 23170, 27554, 32768, 38967, 46340, 55108, 65536,
    77935, 92681, 110217, 131072, 155871, 185363, 220435, 262144, 311743,
    370727, 440871, 524288, 623487, 741455, 881743, 1048576
};

CalypsoPo::CalypsoPo(std::shared_ptr<SeResponse> selectionResponse,
                     const TransmissionMode& transmissionMode)
: AbstractMatchingSe(selectionResponse, transmissionMode)
{
    int bufferSizeIndicator;
    int bufferSizeValue;

    if (hasFci() && static_cast<int>(getFciBytes().size()) > 2) {

        /*
         * Parse PO FCI - to retrieve DF Name (AID), Serial Number, &amp;
         * StartupInfo
         */
        GetDataFciRespPars poFciRespPars(
            selectionResponse->getSelectionStatus()->getFci(), nullptr);

        /* 4 fields extracted by the low level parser */
        mDfName = poFciRespPars.getDfName();
        mCalypsoSerialNumber = poFciRespPars.getApplicationSerialNumber();
        mStartupInfo = poFciRespPars.getDiscretionaryData();
        mIsDfInvalidated = poFciRespPars.isDfInvalidated();

        const uint8_t applicationType = getApplicationType();
        mRevision = determineRevision(applicationType);

        /* Session buffer size */
        bufferSizeIndicator = mStartupInfo[SI_BUFFER_SIZE_INDICATOR];
        bufferSizeValue =
            BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE[bufferSizeIndicator];

        if (mRevision == PoRevision::REV2_4) {
            /*
             * Old cards have their modification counter in number of commands
             */
            mModificationCounterIsInBytes = false;
            mModificationsCounterMax =
                REV2_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION;
        } else {
            mModificationsCounterMax = bufferSizeValue;
        }

        mIsConfidentialSessionModeSupported =
            (applicationType & APP_TYPE_CALYPSO_REV_32_MODE) != 0;
        mIsDeselectRatificationSupported =
            (applicationType & APP_TYPE_RATIFICATION_COMMAND_REQUIRED) == 0;
        mIsSvFeatureAvailable =
            (applicationType & APP_TYPE_WITH_CALYPSO_SV) != 0;
        mIsPinFeatureAvailable =
            (applicationType & APP_TYPE_WITH_CALYPSO_PIN) != 0;
        mIsPublicAuthenticationSupported =
            (applicationType & APP_TYPE_WITH_PUBLIC_AUTHENTICATION) != 0;
    } else {
        /*
         * FCI is not provided: we consider it is Calypso PO rev 1, it's serial
         * number is provided in the ATR
         */
        if (!hasAtr())
            throw IllegalStateException(
                      "Unable to identify this PO: Neither the CFI nor the " \
                      "ATR are available.");

        const std::vector<uint8_t> atr = getAtrBytes();

        /*
         * Basic check: we expect to be here following a selection based on the
         * ATR
         */
        if (static_cast<int>(atr.size()) != PO_REV1_ATR_LENGTH)
            throw IllegalStateException(
                    "Unexpected ATR length: " +
                    ByteArrayUtil::toHex(getAtrBytes()));

        mRevision = PoRevision::REV1_0;

        /* C++: this line below seems useless */
        //mDfName = null;
        mCalypsoSerialNumber = std::vector<uint8_t>(8);


        /* Old cards have their modification counter in number of commands */
        mModificationCounterIsInBytes = false;

        /*
         * The array is initialized with 0 (cf. default value for primitive
         * types)
         */
        System::arraycopy(atr, 12, mCalypsoSerialNumber, 4, 4);
        mModificationsCounterMax =
            REV1_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION;

        mStartupInfo = std::vector<uint8_t>(7);
        /* Create buffer size indicator */
        mStartupInfo[0] = mModificationsCounterMax;

        /*
         * Create the startup info with the 6 bytes of the ATR from position 6
         */
        System::arraycopy(atr, 6, mStartupInfo, 1, 6);

        /* TODO check these flags */
        mIsConfidentialSessionModeSupported = false;
        mIsDeselectRatificationSupported = true;
        mIsSvFeatureAvailable = false;
        mIsPinFeatureAvailable = false;
        mIsPublicAuthenticationSupported = false;
        mIsDfInvalidated = false;
    }

    /*
     * Rev1 and Rev2 expects the legacy class byte while Rev3 expects the ISO
     * class byte
     */
    if (mRevision == PoRevision::REV1_0 || mRevision == PoRevision::REV2_4)
        mPoClass = std::make_shared<PoClass>(PoClass::LEGACY);
    else
        mPoClass = std::make_shared<PoClass>(PoClass::ISO);
}

PoRevision CalypsoPo::getRevision() const
{
    return mRevision;
}

const std::vector<uint8_t>& CalypsoPo::getDfNameBytes() const
{
    return mDfName;
}

const std::string CalypsoPo::getDfName() const
{
    return ByteArrayUtil::toHex(getDfNameBytes());
}

const std::vector<uint8_t>& CalypsoPo::getCalypsoSerialNumber() const
{
    return mCalypsoSerialNumber;
}

const std::vector<uint8_t> CalypsoPo::getApplicationSerialNumberBytes() const
{
    std::vector<uint8_t> applicationSerialNumber = mCalypsoSerialNumber;
    applicationSerialNumber[0] = 0;
    applicationSerialNumber[1] = 0;

    return applicationSerialNumber;
}

const std::string CalypsoPo::getApplicationSerialNumber() const
{
    return ByteArrayUtil::toHex(getApplicationSerialNumberBytes());
}

const std::string CalypsoPo::getStartupInfo() const
{
    return ByteArrayUtil::toHex(mStartupInfo);
}

bool CalypsoPo::isSerialNumberExpiring() const
{
    throw IllegalStateException("Not yet implemented");
}

const std::vector<uint8_t>& CalypsoPo::getSerialNumberExpirationBytes() const
{
    throw IllegalStateException("Not yet implemented");
}

int CalypsoPo::getPayloadCapacity() const
{
    /* TODO make this value dependent on the type of PO identified */
    return 250;
}

const std::string CalypsoPo::getAtr() const
{
    return ByteArrayUtil::toHex(getAtrBytes());
}

bool CalypsoPo::isModificationsCounterInBytes() const
{
    return mModificationCounterIsInBytes;
}

int CalypsoPo::getModificationsCounter() const
{
    return mModificationsCounterMax;
}

uint8_t CalypsoPo::getPlatform() const
{
    return mStartupInfo[SI_PLATFORM];
}

uint8_t CalypsoPo::getApplicationType() const
{
    return mStartupInfo[SI_APPLICATION_TYPE];
}

bool CalypsoPo::isConfidentialSessionModeSupported() const
{
    return mIsConfidentialSessionModeSupported;
}

bool CalypsoPo::isDeselectRatificationSupported() const
{
    return mIsDeselectRatificationSupported;
}

bool CalypsoPo::isSvFeatureAvailable() const
{
    return mIsSvFeatureAvailable;
}

bool CalypsoPo::isPinFeatureAvailable() const
{
    return mIsPinFeatureAvailable;
}

bool CalypsoPo::isPublicAuthenticationSupported() const
{
    return mIsPublicAuthenticationSupported;
}

uint8_t CalypsoPo::getApplicationSubtype() const
{
    return mStartupInfo[SI_APPLICATION_SUBTYPE];
}

uint8_t CalypsoPo::getSoftwareIssuer() const
{
    return mStartupInfo[SI_SOFTWARE_ISSUER];
}

uint8_t CalypsoPo::getSoftwareVersion() const
{
    return mStartupInfo[SI_SOFTWARE_VERSION];
}

uint8_t CalypsoPo::getSoftwareRevision() const
{
    return mStartupInfo[SI_SOFTWARE_REVISION];
}

uint8_t CalypsoPo::getSessionModification() const
{
    return mStartupInfo[SI_BUFFER_SIZE_INDICATOR];
}

bool CalypsoPo::isDfInvalidated() const
{
    return mIsDfInvalidated;
}

bool CalypsoPo::isDfRatified() const
{
    return mIsDfRatified;
}

void CalypsoPo::setDfRatified(const bool dfRatified)
{
    mIsDfRatified = dfRatified;
}

const PoClass& CalypsoPo::getPoClass() const
{
    return *mPoClass.get();
}

const std::shared_ptr<DirectoryHeader> CalypsoPo::getDirectoryHeader() const
{
    return mDirectoryHeader;
}

CalypsoPo* CalypsoPo::setDirectoryHeader(
    const std::shared_ptr<DirectoryHeader> directoryHeader)
{
    mDirectoryHeader = directoryHeader;

    return this;
}

const std::shared_ptr<ElementaryFile> CalypsoPo::getFileBySfi(const uint8_t sfi)
    const
{
    std::map<uint8_t, std::shared_ptr<ElementaryFile>>::const_iterator it;

    if ((it = mEfBySfi.find(sfi)) == mEfBySfi.end())
        throw NoSuchElementException(
                  "EF with SFI [" + StringHelper::uint8ToHexString(sfi) + "]" +
                  " is not found.");

    return it->second;
}

const std::shared_ptr<ElementaryFile> CalypsoPo::getFileByLid(
    const uint16_t lid)
{
    std::map<uint16_t, uint8_t>::const_iterator it;

    if ((it = mSfiByLid.find(lid)) == mSfiByLid.end())
        throw NoSuchElementException(
                  "EF with LID [" + StringHelper::uint16ToHexString(lid) + "]" +
                  " is not found.");

    std::map<uint8_t, std::shared_ptr<ElementaryFile>>::const_iterator it2;

    if ((it2 = mEfBySfi.find(it->second)) == mEfBySfi.end())
        throw NoSuchElementException(
                  "EF with LID [" + StringHelper::uint16ToHexString(lid) + "]" +
                  " is not found.");

    return it2->second;
}

const std::map<uint8_t, std::shared_ptr<ElementaryFile>>&
    CalypsoPo::getAllFiles() const
{
    return mEfBySfi;
}

void CalypsoPo::setFileHeader(const uint8_t sfi,
                              const std::shared_ptr<FileHeader> header)
{
    std::shared_ptr<ElementaryFile> ef = getOrCreateFile(sfi);
    ef->setHeader(header);
    mSfiByLid.insert({header->getLid(), sfi});
}

void CalypsoPo::setContent(const uint8_t sfi,
                           const int numRecord,
                           const std::vector<uint8_t>& content)
{
    std::shared_ptr<ElementaryFile> ef = getOrCreateFile(sfi);
    ef->getData()->setContent(numRecord, content);
}

void CalypsoPo::setCounter(const uint8_t sfi,
                           const int numCounter,
                           const std::vector<uint8_t>& content)
{
    std::shared_ptr<ElementaryFile> ef = getOrCreateFile(sfi);
    ef->getData()->setCounter(numCounter, content);
}

void CalypsoPo::setContent(const uint8_t sfi,
                           const int numRecord,
                           const std::vector<uint8_t>& content,
                           const int offset)
{
    std::shared_ptr<ElementaryFile> ef = getOrCreateFile(sfi);
    ef->getData()->setContent(numRecord, content, offset);
}

void CalypsoPo::fillContent(const uint8_t sfi,
                            const int numRecord,
                            std::vector<uint8_t>& content)
{
    std::shared_ptr<ElementaryFile> ef = getOrCreateFile(sfi);
    ef->getData()->fillContent(numRecord, content);
}

void CalypsoPo::addCyclicContent(const uint8_t sfi,
                                 const std::vector<uint8_t>& content)
{
    std::shared_ptr<ElementaryFile> ef = getOrCreateFile(sfi);
    ef->getData()->addCyclicContent(content);
}

void CalypsoPo::backupFiles()
{
    copyMapFiles(mEfBySfi, mEfBySfiBackup);
    copyMapSfi(mSfiByLid, mSfiByLidBackup);
}

void CalypsoPo::restoreFiles()
{
    copyMapFiles(mEfBySfiBackup, mEfBySfi);
    copyMapSfi(mSfiByLidBackup, mSfiByLid);
}

std::shared_ptr<ElementaryFile> CalypsoPo::getOrCreateFile(const uint8_t sfi)
{
    std::map<uint8_t, std::shared_ptr<ElementaryFile>>::const_iterator it;
    std::shared_ptr<ElementaryFile> ef = nullptr;

    if ((it = mEfBySfi.find(sfi)) == mEfBySfi.end()) {
        ef = std::make_shared<ElementaryFile>(sfi);
        mEfBySfi.insert({sfi, ef});
    } else {
        ef = it->second;
    }

    return ef;
}

void CalypsoPo::copyMapFiles(
    const std::map<uint8_t, std::shared_ptr<ElementaryFile>>& src,
    std::map<uint8_t, std::shared_ptr<ElementaryFile>>& dest)
{
    dest.clear();

    for (const auto& p : src)
        /* Duplicate ElementaryFile and its FileData/FileHeader */
        dest.insert({p.first,
                     std::make_shared<ElementaryFile>(*p.second.get())});
}

void CalypsoPo::copyMapSfi(const std::map<uint16_t, uint8_t>& src,
                           std::map<uint16_t, uint8_t>& dest)
{
    dest = src;
}

PoRevision CalypsoPo::determineRevision(const uint8_t applicationType) const
{
    if ((applicationType & (1 << 7)) != 0) {
        /* CLAP */
        return PoRevision::REV3_1_CLAP;
    } else if ((applicationType >> 3) == 0x05) {
        return PoRevision::REV3_2;
    } else if ((applicationType >> 3) == 0x04) {
        return PoRevision::REV3_1;
    } else {
        return PoRevision::REV2_4;
    }
}

}
}
}
