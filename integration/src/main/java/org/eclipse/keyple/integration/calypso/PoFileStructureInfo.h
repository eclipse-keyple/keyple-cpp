#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace integration { namespace calypso { class EfData; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeResponse; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class MatchingSe; } } } }

/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace calypso {


                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using MatchingSe = org::eclipse::keyple::transaction::MatchingSe;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.VariableNamingConventions") public class PoFileStructureInfo
                    class PoFileStructureInfo : public std::enable_shared_from_this<PoFileStructureInfo> {

                    public:
                        static const std::string poAuditC0Aid; // AID of the Rev3 PO with
                                                                                          // Audit C0 profile
                        static const std::string clapAid; // AID of the CLAP product
                                                                                             // being tested
                        static const std::string cdLightAid; // AID of the Rev2.4 PO emulating
                                                                                    // CDLight

                    private:
                        std::shared_ptr<EfData> environmentFileData;

                        std::shared_ptr<EfData> eventFileData;

                        std::shared_ptr<EfData> contractListFileData;

                        std::shared_ptr<EfData> contractFileData;

                        std::shared_ptr<EfData> countersFileData;

                        std::shared_ptr<EfData> simulatedCountersFileData;

                        std::shared_ptr<SeResponse> selectionData;

                        std::shared_ptr<MatchingSe> matchingSe;

                        void setFileStructureForAuditC0();


                        void setFileStructureForClap();


                        void setFileStructureForCdLight();

                    public:
                        PoFileStructureInfo(std::shared_ptr<MatchingSe> matchingSe);

                        virtual std::shared_ptr<EfData> getEnvironmentFileData();

                        virtual std::shared_ptr<EfData> getEventFileData();

                        virtual std::shared_ptr<EfData> getContractListFileData();

                        virtual std::shared_ptr<EfData> getContractFileData();

                        virtual std::shared_ptr<EfData> getCountersFileData();

                        virtual std::shared_ptr<EfData> getSimulatedCountersFileData();

                        virtual std::shared_ptr<SeResponse> getSelectionData();

                        virtual std::shared_ptr<MatchingSe> getMatchingSe();
                    };

                }
            }
        }
    }
}
