#include "DigestAuthenticateCmdBuildTest.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/sam/builder/session/DigestAuthenticateCmdBuild.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduCommandBuilder.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace builder {
                            using DigestAuthenticateCmdBuild = org::eclipse::keyple::calypso::command::sam::builder::session::DigestAuthenticateCmdBuild;
                            using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                            using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                            using org::junit::Assert;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void digestAuthenticate() throws IllegalArgumentException
                            void DigestAuthenticateCmdBuildTest::digestAuthenticate() throw(std::invalid_argument) {

                                std::vector<char> signaturePO = {0x00, 0x01, 0x02, 0x03};
                                std::vector<char> request = {static_cast<char>(0x94), static_cast<char>(0x82), 0x00, 0x00, 0x04, 0x00, 0x01, 0x02, 0x03};

                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<DigestAuthenticateCmdBuild>(nullptr, signaturePO);
                                ApduRequest ApduRequest = apduCommandBuilder->getApduRequest();

                                Assert::assertEquals(ByteArrayUtils::toHex(request), ByteArrayUtils::toHex(ApduRequest::getBytes()));
                            }
                        }
                    }
                }
            }
        }
    }
}
