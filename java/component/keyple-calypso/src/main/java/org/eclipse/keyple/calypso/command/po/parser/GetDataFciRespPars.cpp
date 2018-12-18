#include "GetDataFciRespPars.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                            using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
const std::unordered_map<Integer, std::shared_ptr<StatusProperties>> GetDataFciRespPars::STATUS_TABLE;

                            GetDataFciRespPars::StaticConstructor::StaticConstructor() {
                                                                    std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
                                                                    m.emplace(0x6A88, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Data object not found (optional mode not available)."));
                                                                    m.emplace(0x6B00, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "P1 or P2 value not supported (<>004fh, 0062h, 006Fh, 00C0h, 00D0h, 0185h and 5F52h, according to availabl optional modes)."));
                                                                    m.emplace(0x6283, std::make_shared<AbstractApduResponseParser::StatusProperties>(true, "Successful execution, FCI request and DF is invalidated."));
                                                                    STATUS_TABLE = m;
                            }

GetDataFciRespPars::StaticConstructor GetDataFciRespPars::staticConstructor;
std::vector<int> const GetDataFciRespPars::bufferSizeIndicatorToBufferSize = std::vector<int> {0, 0, 0, 0, 0, 0, 215, 256, 304, 362, 430, 512, 608, 724, 861, 1024, 1217, 1448, 1722, 2048, 2435, 2896, 3444, 4096, 4870, 5792, 6888, 8192, 9741, 11585, 13777, 16384, 19483, 23170, 27554, 32768, 38967, 46340, 55108, 65536, 77935, 92681, 110217, 131072, 155871, 185363, 220435, 262144, 311743, 370727, 440871, 524288, 623487, 741455, 881743, 1048576};

                            std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> GetDataFciRespPars::getStatusTable() {
                                return STATUS_TABLE;
                            }

                            GetDataFciRespPars::GetDataFciRespPars(std::shared_ptr<ApduResponse> response) : org::eclipse::keyple::command::AbstractApduResponseParser(response), fci(isSuccessful() ? toFCI(response->getBytes()) : nullptr) {
                            }

                            std::vector<char> GetDataFciRespPars::getDfName() {
                                return fci != nullptr ? fci->getDfName() : nullptr;
                            }

                            std::vector<char> GetDataFciRespPars::getApplicationSerialNumber() {
                                return fci != nullptr ? fci->getApplicationSN() : nullptr;
                            }

                            char GetDataFciRespPars::getBufferSizeIndicator() {
                                return fci != nullptr ? fci->getStartupInformation()->getBufferSizeIndicator() : 0x00;
                            }

                            int GetDataFciRespPars::getBufferSizeValue() {
                                return fci != nullptr ? bufferSizeIndicatorToBufferSize[static_cast<int>(fci->getStartupInformation()->getBufferSizeIndicator())] : 0;
                            }

                            char GetDataFciRespPars::getPlatformByte() {
                                return fci != nullptr ? fci->getStartupInformation()->getPlatform() : 0x00;
                            }

                            char GetDataFciRespPars::getApplicationTypeByte() {
                                return fci != nullptr ? fci->getStartupInformation()->getApplicationType() : 0x00;
                            }

                            bool GetDataFciRespPars::isRev3Compliant() {
                                return fci != nullptr;
                            }

                            bool GetDataFciRespPars::isRev3_2ModeAvailable() {
                                return fci != nullptr && fci->getStartupInformation()->hasCalypsoRev32modeAvailable();
                            }

                            bool GetDataFciRespPars::isRatificationCommandRequired() {
                                return fci != nullptr && fci->getStartupInformation()->hasRatificationCommandRequired();
                            }

                            bool GetDataFciRespPars::hasCalypsoStoredValue() {
                                return fci != nullptr && fci->getStartupInformation()->hasCalypsoStoreValue();
                            }

                            bool GetDataFciRespPars::hasCalypsoPin() {
                                return fci != nullptr && fci->getStartupInformation()->hasCalypsoPin();
                            }

                            char GetDataFciRespPars::getApplicationSubtypeByte() {
                                return fci != nullptr ? fci->getStartupInformation()->getApplicationSubtype() : 0x00;
                            }

                            char GetDataFciRespPars::getSoftwareIssuerByte() {
                                return fci != nullptr ? fci->getStartupInformation()->getSoftwareIssuer() : 0x00;
                            }

                            char GetDataFciRespPars::getSoftwareVersionByte() {
                                return fci != nullptr ? fci->getStartupInformation()->getSoftwareVersion() : 0x00;
                            }

                            char GetDataFciRespPars::getSoftwareRevisionByte() {
                                return fci != nullptr ? fci->getStartupInformation()->getSoftwareRevision() : 0x00;
                            }

                            bool GetDataFciRespPars::isDfInvalidated() {
                                return fci != nullptr;
                            }

                            GetDataFciRespPars::FCI::FCI(std::vector<char> &dfName, std::vector<char> &fciProprietaryTemplate, std::vector<char> &fciIssuerDiscretionaryData, std::vector<char> &applicationSN, std::shared_ptr<StartupInformation> startupInformation) : dfName(dfName), fciProprietaryTemplate(fciProprietaryTemplate), fciIssuerDiscretionaryData(fciIssuerDiscretionaryData), applicationSN(applicationSN), startupInformation(startupInformation) {
                            }

                            std::vector<char> GetDataFciRespPars::FCI::getFciProprietaryTemplate() {
                                return fciProprietaryTemplate;
                            }

                            std::vector<char> GetDataFciRespPars::FCI::getFciIssuerDiscretionaryData() {
                                return fciIssuerDiscretionaryData;
                            }

                            std::vector<char> GetDataFciRespPars::FCI::getApplicationSN() {
                                return applicationSN;
                            }

                            std::shared_ptr<StartupInformation> GetDataFciRespPars::FCI::getStartupInformation() {
                                return this->startupInformation;
                            }

                            std::vector<char> GetDataFciRespPars::FCI::getDfName() {
                                return dfName;
                            }

                            GetDataFciRespPars::StartupInformation::StartupInformation(char bufferSizeIndicator, char platform, char applicationType, char applicationSubtype, char softwareIssuer, char softwareVersion, char softwareRevision) : bufferSizeIndicator(bufferSizeIndicator), platform(platform), applicationType(applicationType), applicationSubtype(applicationSubtype), softwareIssuer(softwareIssuer), softwareVersion(softwareVersion), softwareRevision(softwareRevision) {
                            }

                            GetDataFciRespPars::StartupInformation::StartupInformation(std::vector<char> &buffer) : bufferSizeIndicator(buffer[0]), platform(buffer[1]), applicationType(buffer[2]), applicationSubtype(buffer[3]), softwareIssuer(buffer[4]), softwareVersion(buffer[5]), softwareRevision(buffer[6]) {
                            }

                            std::shared_ptr<StartupInformation> GetDataFciRespPars::StartupInformation::empty() {
                                return std::make_shared<StartupInformation>(static_cast<char>(0), static_cast<char>(0), static_cast<char>(0), static_cast<char>(0), static_cast<char>(0), static_cast<char>(0), static_cast<char>(0));
                            }

                            int GetDataFciRespPars::StartupInformation::hashCode() {
                                constexpr int prime = 31;
                                int result = 1;
                                result = prime * result + applicationSubtype;
                                result = prime * result + applicationType;
                                result = prime * result + bufferSizeIndicator;
                                result = prime * result + platform;
                                result = prime * result + softwareIssuer;
                                result = prime * result + softwareRevision;
                                result = prime * result + softwareVersion;
                                return result;
                            }

                            bool GetDataFciRespPars::StartupInformation::equals(std::shared_ptr<void> obj) {

                                bool isEquals = false;

                                if (shared_from_this() == obj) {
                                    isEquals = true;
                                }

                                if (obj != nullptr) {
                                    std::shared_ptr<StartupInformation> other;
                                    if (getClass() == obj->type()) {
                                        other = std::static_pointer_cast<StartupInformation>(obj);
                                        if ((applicationSubtype != other->applicationSubtype) || (applicationType != other->applicationType) || (bufferSizeIndicator != other->bufferSizeIndicator) || (platform != other->platform) || (softwareIssuer != other->softwareIssuer) || (softwareRevision != other->softwareRevision) || (softwareVersion != other->softwareVersion)) {
                                            isEquals = false;
                                        }
                                    }
                                    else {
                                        isEquals = false;
                                    }

                                }
                                else {
                                    isEquals = false;
                                }

                                return isEquals;
                            }

                            char GetDataFciRespPars::StartupInformation::getBufferSizeIndicator() {
                                return bufferSizeIndicator;
                            }

                            char GetDataFciRespPars::StartupInformation::getPlatform() {
                                return platform;
                            }

                            char GetDataFciRespPars::StartupInformation::getApplicationType() {
                                return applicationType;
                            }

                            char GetDataFciRespPars::StartupInformation::getApplicationSubtype() {
                                return applicationSubtype;
                            }

                            char GetDataFciRespPars::StartupInformation::getSoftwareIssuer() {
                                return softwareIssuer;
                            }

                            char GetDataFciRespPars::StartupInformation::getSoftwareVersion() {
                                return softwareVersion;
                            }

                            char GetDataFciRespPars::StartupInformation::getSoftwareRevision() {
                                return softwareRevision;
                            }

                            bool GetDataFciRespPars::StartupInformation::hasCalypsoPin() {
                                return (this->applicationType & 0x01) != 0;
                            }

                            bool GetDataFciRespPars::StartupInformation::hasCalypsoStoreValue() {
                                return (this->applicationType & 0x02) != 0;
                            }

                            bool GetDataFciRespPars::StartupInformation::hasRatificationCommandRequired() {
                                return (this->applicationType & 0x04) != 0;
                            }

                            bool GetDataFciRespPars::StartupInformation::hasCalypsoRev32modeAvailable() {
                                return (this->applicationType & 0x08) != 0;
                            }

                            std::shared_ptr<FCI> GetDataFciRespPars::toFCI(std::vector<char> &apduResponse) {
                                std::shared_ptr<StartupInformation> startupInformation = StartupInformation::empty();
                                char firstResponseApdubyte = apduResponse[0];
                                std::vector<char> dfName;
                                std::vector<char> fciProprietaryTemplate;
                                std::vector<char> fciIssuerDiscretionaryData;
                                std::vector<char> applicationSN;
                                std::vector<char> discretionaryData;

                                if (static_cast<char>(0x6F) == firstResponseApdubyte) {
                                    int aidLength = apduResponse[3];
                                    int fciTemplateLength = apduResponse[5 + aidLength];
                                    int fixedPartOfFciTemplate = fciTemplateLength - 22;
                                    int firstbyteAid = 6 + aidLength + fixedPartOfFciTemplate;
                                    int fciIssuerDiscretionaryDataLength = apduResponse[8 + aidLength + fixedPartOfFciTemplate];
                                    int firstbyteFciIssuerDiscretionaryData = 9 + aidLength + fixedPartOfFciTemplate;
                                    int applicationSNLength = apduResponse[10 + aidLength + fixedPartOfFciTemplate];
                                    int firstbyteApplicationSN = 11 + aidLength + fixedPartOfFciTemplate;
                                    int discretionaryDataLength = apduResponse[20 + aidLength + fixedPartOfFciTemplate];
                                    int firstbyteDiscretionaryData = 21 + aidLength + fixedPartOfFciTemplate;

                                    if (static_cast<char>(0x84) == apduResponse[2]) {
                                        dfName = Arrays::copyOfRange(apduResponse, 4, 4 + aidLength);
                                    }

                                    if (static_cast<char>(0xA5) == apduResponse[4 + aidLength]) {
                                        fciProprietaryTemplate = Arrays::copyOfRange(apduResponse, firstbyteAid, firstbyteAid + fciTemplateLength);
                                    }

                                    if (static_cast<char>(0xBF) == apduResponse[6 + aidLength + fixedPartOfFciTemplate] && (static_cast<char>(0x0C) == apduResponse[7 + aidLength + fixedPartOfFciTemplate])) {
                                        fciIssuerDiscretionaryData = Arrays::copyOfRange(apduResponse, firstbyteFciIssuerDiscretionaryData, firstbyteFciIssuerDiscretionaryData + fciIssuerDiscretionaryDataLength);
                                    }

                                    if (static_cast<char>(0xC7) == apduResponse[9 + aidLength + fixedPartOfFciTemplate]) {
                                        applicationSN = Arrays::copyOfRange(apduResponse, firstbyteApplicationSN, firstbyteApplicationSN + applicationSNLength);
                                    }

                                    if (static_cast<char>(0x53) == apduResponse[19 + aidLength + fixedPartOfFciTemplate]) {
                                        discretionaryData = Arrays::copyOfRange(apduResponse, firstbyteDiscretionaryData, firstbyteDiscretionaryData + discretionaryDataLength);
                                        startupInformation = std::make_shared<StartupInformation>(discretionaryData);
                                        /*
                                         * startupInformation = new StartupInformation(discretionaryData.get(0),
                                         * discretionaryData.get(1), discretionaryData.get(2), discretionaryData.get(3),
                                         * discretionaryData.get(4), discretionaryData.get(5), discretionaryData.get(6));
                                         */
                                    }
                                }

                                return std::make_shared<FCI>(dfName, fciProprietaryTemplate, fciIssuerDiscretionaryData, applicationSN, startupInformation);
                            }
                        }
                    }
                }
            }
        }
    }
}
