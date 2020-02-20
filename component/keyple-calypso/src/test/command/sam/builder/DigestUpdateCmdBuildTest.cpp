#include "DigestUpdateCmdBuildTest.h"
//#include "SamRevision.h"
#include "DigestUpdateCmdBuild.h"
#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"

using namespace keyple::calypso::command::sam::builder::security;

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace builder {
                            namespace security {

                            using SamRevision = keyple::calypso::command::sam::SamRevision;
                            using DigestUpdateCmdBuild = keyple::calypso::command::sam::builder::security::DigestUpdateCmdBuild;
                            using AbstractApduCommandBuilder = keyple::core::command::AbstractApduCommandBuilder;
                            using ApduRequest = keyple::core::seproxy::message::ApduRequest;


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void digestUpdateCmdBuild() throws IllegalArgumentException
                            void DigestUpdateCmdBuildTest::digestUpdateCmdBuild() {
                                std::vector<char> digestDAta = {static_cast<char>(0x94), static_cast<char>(0xAE), 0x01, 0x02};
                                std::vector<char> request = {static_cast<char>(0x94), static_cast<char>(0x8C), 0x00, static_cast<char>(0x80), static_cast<char>(digestDAta.size()), static_cast<char>(0x94), static_cast<char>(0xAE), 0x01, 0x02};

                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<DigestUpdateCmdBuild>(SamRevision::S1D, true, digestDAta);
                                std::shared_ptr<ApduRequest> apduRequest = apduCommandBuilder->getApduRequest();

                                ASSERT_EQ(request, apduRequest->getBytes());

                                std::vector<char> request2 = {static_cast<char>(0x80), static_cast<char>(0x8C), 0x00, static_cast<char>(0x80), static_cast<char>(digestDAta.size()), static_cast<char>(0x94), static_cast<char>(0xAE), 0x01, 0x02};

                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder2 = std::make_shared<DigestUpdateCmdBuild>(SamRevision::C1, true, digestDAta);
                                std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder2->getApduRequest();
                                ASSERT_EQ(request2, apduReq->getBytes());
                                }
                            }
                        }
                    }
                }
            }
        }
