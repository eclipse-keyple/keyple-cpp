#include "KeypleDtoHelperTest.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleDto.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleDtoHelper.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace common {
                        namespace json {
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using KeypleDtoHelper = org::eclipse::keyple::plugin::remotese::transport::KeypleDtoHelper;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> KeypleDtoHelperTest::logger = org::slf4j::LoggerFactory::getLogger(KeypleDtoHelperTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testContainsException()
                            void KeypleDtoHelperTest::testContainsException() {

                                std::runtime_error ex = std::make_shared<KeypleReaderException>("keyple Reader Exception message", std::make_shared<IOException>("error io"));
                                std::shared_ptr<KeypleDto> dtoWithException = KeypleDtoHelper::ExceptionDTO("any", ex, "any", "any", "any", "any");
                                logger->debug(KeypleDtoHelper::toJson(dtoWithException));
                                assert(KeypleDtoHelper::containsException(dtoWithException));


                            }
                        }
                    }
                }
            }
        }
    }
}
