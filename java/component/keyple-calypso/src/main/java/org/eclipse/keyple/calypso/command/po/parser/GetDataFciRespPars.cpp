#include "GetDataFciRespPars.h"
#include "../../../../../../../opencard/opt/util/Tag.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../opencard/opt/util/TLV.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                            using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using TLV = opencard::opt::util::TLV;
                            using Tag = opencard::opt::util::Tag;
const std::shared_ptr<org::slf4j::Logger> GetDataFciRespPars::logger = org::slf4j::LoggerFactory::getLogger(GetDataFciRespPars::typeid);
const std::unordered_map<Integer, std::shared_ptr<StatusProperties>> GetDataFciRespPars::STATUS_TABLE;

                            GetDataFciRespPars::StaticConstructor::StaticConstructor() {
                                                                    std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
                                                                    m.emplace(0x6A88, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Data object not found (optional mode not available)."));
                                                                    m.emplace(0x6B00, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "P1 or P2 value not supported (<>004fh, 0062h, 006Fh, 00C0h, 00D0h, 0185h and 5F52h, according to availabl optional modes)."));
                                                                    m.emplace(0x6283, std::make_shared<AbstractApduResponseParser::StatusProperties>(true, "Successful execution, FCI request and DF is invalidated."));
                                                                    STATUS_TABLE = m;
                            }

GetDataFciRespPars::StaticConstructor GetDataFciRespPars::staticConstructor;

                            std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> GetDataFciRespPars::getStatusTable() {
                                return STATUS_TABLE;
                            }

std::vector<int> const GetDataFciRespPars::BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE = std::vector<int> {0, 0, 0, 0, 0, 0, 215, 256, 304, 362, 430, 512, 608, 724, 861, 1024, 1217, 1448, 1722, 2048, 2435, 2896, 3444, 4096, 4870, 5792, 6888, 8192, 9741, 11585, 13777, 16384, 19483, 23170, 27554, 32768, 38967, 46340, 55108, 65536, 77935, 92681, 110217, 131072, 155871, 185363, 220435, 262144, 311743, 370727, 440871, 524288, 623487, 741455, 881743, 1048576};
const std::shared_ptr<opencard::opt::util::Tag> GetDataFciRespPars::TAG_FCI_TEMPLATE = std::make_shared<opencard::opt::util::Tag>(0x0F, opencard::opt::util::Tag::APPLICATION, opencard::opt::util::Tag::CONSTRUCTED);
const std::shared_ptr<opencard::opt::util::Tag> GetDataFciRespPars::TAG_DF_NAME = std::make_shared<opencard::opt::util::Tag>(0x04, opencard::opt::util::Tag::CONTEXT, opencard::opt::util::Tag::PRIMITIVE);
const std::shared_ptr<opencard::opt::util::Tag> GetDataFciRespPars::TAG_FCI_PROPRIETARY_TEMPLATE = std::make_shared<opencard::opt::util::Tag>(0x05, opencard::opt::util::Tag::CONTEXT, opencard::opt::util::Tag::CONSTRUCTED);
const std::shared_ptr<opencard::opt::util::Tag> GetDataFciRespPars::TAG_FCI_ISSUER_DISCRETIONARY_DATA = std::make_shared<opencard::opt::util::Tag>(0x0C, opencard::opt::util::Tag::CONTEXT, opencard::opt::util::Tag::CONSTRUCTED);
const std::shared_ptr<opencard::opt::util::Tag> GetDataFciRespPars::TAG_APPLICATION_SERIAL_NUMBER = std::make_shared<opencard::opt::util::Tag>(0x07, opencard::opt::util::Tag::PRIVATE, opencard::opt::util::Tag::PRIMITIVE);
const std::shared_ptr<opencard::opt::util::Tag> GetDataFciRespPars::TAG_DISCRETIONARY_DATA = std::make_shared<opencard::opt::util::Tag>(0x13, opencard::opt::util::Tag::APPLICATION, opencard::opt::util::Tag::PRIMITIVE);

                            GetDataFciRespPars::GetDataFciRespPars(std::shared_ptr<ApduResponse> selectApplicationResponse) {

                                std::shared_ptr<TLV> cTag; // constructed tag
                                std::shared_ptr<TLV> pTag; // primitive tag

                                /* check the command status to determine if the DF has been invalidated */
                                if (selectApplicationResponse->getStatusCode() == 0x6283) {
                                    logger->debug("The response to the select application command status word indicates that the DF has been invalidated.");
                                    isDfInvalidated_Renamed = true;
                                }

                                /* parse the raw data with the help of the TLV class */
                                try {
                                    /* init TLV object */
                                    cTag = std::make_shared<TLV>(selectApplicationResponse->getDataOut());
                                    if (cTag != nullptr && cTag->tag()->equals(TAG_FCI_TEMPLATE)) {
                                        pTag = cTag->findTag(TAG_DF_NAME, nullptr);
                                        if (pTag != nullptr) {
                                            /* store dfName */
                                            dfName = pTag->valueAsByteArray();
                                            if (logger->isDebugEnabled()) {
                                                logger->debug("DF Name = {}", ByteArrayUtils::toHex(dfName));
                                            }
                                            cTag = cTag->findTag(TAG_FCI_PROPRIETARY_TEMPLATE, nullptr);
                                            if (cTag != nullptr) {
                                                cTag = cTag->findTag(TAG_FCI_ISSUER_DISCRETIONARY_DATA, nullptr);
                                                if (cTag != nullptr) {
                                                    pTag = cTag->findTag(TAG_APPLICATION_SERIAL_NUMBER, nullptr);
                                                    if (pTag != nullptr) {
                                                        /* store Application Serial Number */
                                                        applicationSN = pTag->valueAsByteArray();
                                                        if (logger->isDebugEnabled()) {
                                                            logger->debug("Application Serial Number = {}", ByteArrayUtils::toHex(applicationSN));
                                                        }
                                                        pTag = cTag->findTag(TAG_DISCRETIONARY_DATA, nullptr);
                                                        if (cTag != nullptr) {
                                                            discretionaryData = pTag->valueAsByteArray();
                                                            if (logger->isDebugEnabled()) {
                                                                logger->debug("Discretionary Data = {}", ByteArrayUtils::toHex(discretionaryData));
                                                            }
                                                            /*
                                                             * split discretionary data in as many individual startup
                                                             * information
                                                             */
                                                            siBufferSizeIndicator = discretionaryData[0];
                                                            siPlatform = discretionaryData[1];
                                                            siApplicationType = discretionaryData[2];
                                                            siApplicationSubtype = discretionaryData[3];
                                                            siSoftwareIssuer = discretionaryData[4];
                                                            siSoftwareVersion = discretionaryData[5];
                                                            siSoftwareRevision = discretionaryData[6];
                                                            /* all 3 main fields were retrieved */
                                                            isValidCalypsoFCI_Renamed = true;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                catch (const std::runtime_error &e) {
                                    /* Silently ignore problems decoding TLV structure */
                                }
                            }

                            bool GetDataFciRespPars::isValidCalypsoFCI() {
                                return isValidCalypsoFCI_Renamed;
                            }

                            std::vector<char> GetDataFciRespPars::getDfName() {
                                return dfName;
                            }

                            std::vector<char> GetDataFciRespPars::getApplicationSerialNumber() {
                                return applicationSN;
                            }

                            char GetDataFciRespPars::getBufferSizeIndicator() {
                                return siBufferSizeIndicator;
                            }

                            int GetDataFciRespPars::getBufferSizeValue() {
                                return BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE[getBufferSizeIndicator()];
                            }

                            char GetDataFciRespPars::getPlatformByte() {
                                return siPlatform;
                            }

                            char GetDataFciRespPars::getApplicationTypeByte() {
                                return siApplicationType;
                            }

                            bool GetDataFciRespPars::isRev3_2ModeAvailable() {
                                return (siApplicationType & APP_TYPE_CALYPSO_REV_32_MODE) != 0;
                            }

                            bool GetDataFciRespPars::isRatificationCommandRequired() {
                                return (siApplicationSubtype & APP_TYPE_RATIFICATION_COMMAND_REQUIRED) != 0;
                            }

                            bool GetDataFciRespPars::hasCalypsoStoredValue() {
                                return (siApplicationSubtype & APP_TYPE_WITH_CALYPSO_SV) != 0;
                            }

                            bool GetDataFciRespPars::hasCalypsoPin() {
                                return (siApplicationSubtype & APP_TYPE_WITH_CALYPSO_PIN) != 0;
                            }

                            char GetDataFciRespPars::getApplicationSubtypeByte() {
                                return siApplicationSubtype;
                            }

                            char GetDataFciRespPars::getSoftwareIssuerByte() {
                                return siSoftwareIssuer;
                            }

                            char GetDataFciRespPars::getSoftwareVersionByte() {
                                return siSoftwareVersion;
                            }

                            char GetDataFciRespPars::getSoftwareRevisionByte() {
                                return siSoftwareRevision;
                            }

                            bool GetDataFciRespPars::isDfInvalidated() {
                                return isDfInvalidated_Renamed;
                            }
                        }
                    }
                }
            }
        }
    }
}
