#include "PoFileStructureInfo.h"
#include "EfData.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/MatchingSe.h"
#include "../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/GetDataFciRespPars.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace calypso {
                    using GetDataFciRespPars = org::eclipse::keyple::calypso::command::po::parser::GetDataFciRespPars;
                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using MatchingSe = org::eclipse::keyple::transaction::MatchingSe;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
const std::string PoFileStructureInfo::poAuditC0Aid = "315449432E4943414C54";
const std::string PoFileStructureInfo::clapAid = "315449432E494341D62010029101";
const std::string PoFileStructureInfo::cdLightAid = "315449432E494341";

                    void PoFileStructureInfo::setFileStructureForAuditC0() {

                        environmentFileData = std::make_shared<EfData>(1, static_cast<char>(0x07), 29);
                        eventFileData = std::make_shared<EfData>(3, static_cast<char>(0x08), 29);
                        contractListFileData = std::make_shared<EfData>(20, static_cast<char>(0x1E), 10);
                        contractFileData = std::make_shared<EfData>(4, static_cast<char>(0x09), 29);
                        countersFileData = std::make_shared<EfData>(1, static_cast<char>(0x19), 29);
                        simulatedCountersFileData = std::make_shared<EfData>(0, static_cast<char>(0x00), 0);

                    }

                    void PoFileStructureInfo::setFileStructureForClap() {

                        environmentFileData = std::make_shared<EfData>(1, static_cast<char>(0x14), 32);
                        eventFileData = std::make_shared<EfData>(2, static_cast<char>(0x08), 64);
                        contractListFileData = std::make_shared<EfData>(2, static_cast<char>(0x1A), 48);
                        contractFileData = std::make_shared<EfData>(2, static_cast<char>(0x15), 64);
                        countersFileData = std::make_shared<EfData>(1, static_cast<char>(0x1B), 6);
                        simulatedCountersFileData = std::make_shared<EfData>(0, static_cast<char>(0x00), 0);

                    }

                    void PoFileStructureInfo::setFileStructureForCdLight() {

                        environmentFileData = std::make_shared<EfData>(1, static_cast<char>(0x07), 29);
                        eventFileData = std::make_shared<EfData>(3, static_cast<char>(0x08), 29);
                        contractListFileData = std::make_shared<EfData>(1, static_cast<char>(0x1E), 29);
                        contractFileData = std::make_shared<EfData>(4, static_cast<char>(0x09), 29);
                        countersFileData = std::make_shared<EfData>(1, static_cast<char>(0x19), 29);
                        simulatedCountersFileData = std::make_shared<EfData>(9, static_cast<char>(0x0A), 3);

                    }

                    PoFileStructureInfo::PoFileStructureInfo(std::shared_ptr<MatchingSe> matchingSe) {

                        this->matchingSe = matchingSe;

                        selectionData = matchingSe->getSelectionSeResponse();

                        std::shared_ptr<GetDataFciRespPars> poFciRespPars = std::make_shared<GetDataFciRespPars>(selectionData->getSelectionStatus()->getFci());
                        std::vector<char> poCalypsoInstanceAid = poFciRespPars->getDfName();

                        if (Arrays::equals(poCalypsoInstanceAid, ByteArrayUtils::fromHex(poAuditC0Aid))) {

                            setFileStructureForAuditC0();

                        }
                        else if (Arrays::equals(poCalypsoInstanceAid, ByteArrayUtils::fromHex(clapAid))) {

                            setFileStructureForClap();

                        }
                        else if (Arrays::equals(poCalypsoInstanceAid, ByteArrayUtils::fromHex(cdLightAid))) {

                            setFileStructureForCdLight();

                        }
                        else {
                            throw std::invalid_argument("The file structure for AID " + ByteArrayUtils::toHex(poCalypsoInstanceAid) + " is not registered for testing.");
                        }
                    }

                    std::shared_ptr<EfData> PoFileStructureInfo::getEnvironmentFileData() {
                        return environmentFileData;
                    }

                    std::shared_ptr<EfData> PoFileStructureInfo::getEventFileData() {
                        return eventFileData;
                    }

                    std::shared_ptr<EfData> PoFileStructureInfo::getContractListFileData() {
                        return contractListFileData;
                    }

                    std::shared_ptr<EfData> PoFileStructureInfo::getContractFileData() {
                        return contractFileData;
                    }

                    std::shared_ptr<EfData> PoFileStructureInfo::getCountersFileData() {
                        return countersFileData;
                    }

                    std::shared_ptr<EfData> PoFileStructureInfo::getSimulatedCountersFileData() {
                        return simulatedCountersFileData;
                    }

                    std::shared_ptr<SeResponse> PoFileStructureInfo::getSelectionData() {
                        return selectionData;
                    }

                    std::shared_ptr<MatchingSe> PoFileStructureInfo::getMatchingSe() {
                        return matchingSe;
                    }
                }
            }
        }
    }
}
