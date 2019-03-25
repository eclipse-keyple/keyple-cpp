#include "GetDataFciRespPars.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
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
                            using com::sun::jndi::ldap::BerDecoder;
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

                            GetDataFciRespPars::GetDataFciRespPars(std::shared_ptr<ApduResponse> selectApplicationResponse) {
                                const std::vector<char> response = selectApplicationResponse->getBytes();
                                std::vector<char> octetString;
                                std::vector<int> rlen(1);
                                std::shared_ptr<BerDecoder> ber;

                                /* check the command status to determine if the DF has been invalidated */
                                if (selectApplicationResponse->getStatusCode() == 0x6283) {
                                    logger->debug("The response to the select application command status word indicates that the DF has been invalidated.");
                                    isDfInvalidated_Renamed = true;
                                }

                                /* parse the raw data with the help of the BerDecoder class */
                                try {
                                    /* init BerDecoder object */
                                    ber = std::make_shared<BerDecoder>(response, 0, response.size());

                                    /* Extract the FCI Template */
                                    octetString = ber->parseOctetString(TAG_FCI_TEMPLATE, nullptr);

                                    ber = std::make_shared<BerDecoder>(octetString, 0, octetString.size());

                                    /* Get the DF Name */
                                    dfName = ber->parseOctetString(TAG_DF_NAME, rlen);

                                    /* Get the FCI Proprietary Template */
                                    ber = std::make_shared<BerDecoder>(octetString, rlen[0], octetString.size());

                                    octetString = ber->parseOctetString(TAG_FCI_PROPRIETARY_TEMPLATE, rlen);

                                    /* Get the FCI Issuer Discretionary Data */
                                    ber = std::make_shared<BerDecoder>(octetString, 0, octetString.size());

                                    /*
                                     * We process the TAG_FCI_ISSUER_DISCRETIONARY_DATA tag here in a particular way since
                                     * the BerDecoder we use does not support 2-byte tags. We first check the two bytes of
                                     * the TAG field and then skip the LENGTH field using an offset value of 3 to obtain the
                                     * following data.
                                     */
                                    char b = static_cast<char>(ber->parseByte());
                                    if (b != static_cast<char>(TAG_FCI_ISSUER_DISCRETIONARY_DATA >> 8)) {
                                        throw std::make_shared<IllegalStateException>(std::string::format("Encountered ASN.1 tag %d (expected tag %d)", b, TAG_FCI_ISSUER_DISCRETIONARY_DATA >> 8));
                                    }

                                    b = static_cast<char>(ber->parseByte());
                                    if (b != static_cast<char>(TAG_FCI_ISSUER_DISCRETIONARY_DATA & 0xFF)) {
                                        throw std::make_shared<IllegalStateException>(std::string::format("Encountered ASN.1 tag %d (expected tag %d)", b, TAG_FCI_ISSUER_DISCRETIONARY_DATA & 0xFF));
                                    }

                                    ber = std::make_shared<BerDecoder>(octetString, 3, octetString.size());

                                    /* Get the Application Serial Number */
                                    applicationSN = ber->parseOctetString(TAG_APPLICATION_SERIAL_NUMBER, rlen);

                                    if (logger->isDebugEnabled()) {
                                        logger->debug("Application Serial Number = {}", ByteArrayUtils::toHex(applicationSN));
                                    }

                                    /* Get the Discretionary Data */
                                    discretionaryData = ber->parseOctetString(TAG_DISCRETIONARY_DATA, nullptr);

                                    if (logger->isDebugEnabled()) {
                                        logger->debug("Discretionary Data = {}", ByteArrayUtils::toHex(discretionaryData));
                                    }

                                    /*
                                     * split discretionary data in as many individual startup information
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
                                catch (const std::runtime_error &e) {
                                    /* Silently ignore problems decoding TLV structure. Just log. */
                                    logger->debug("Error while parsing the FCI BER-TLV data structure ({})", e.what());
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
