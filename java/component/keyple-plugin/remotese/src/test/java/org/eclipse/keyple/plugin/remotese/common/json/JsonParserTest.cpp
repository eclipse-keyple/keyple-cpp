#include "JsonParserTest.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "SampleFactory.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/transaction/SelectionRequest.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/json/JsonParser.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace common {
                        namespace json {
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                            using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                            using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                            using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::google::gson::Gson;
const std::shared_ptr<org::slf4j::Logger> JsonParserTest::logger = org::slf4j::LoggerFactory::getLogger(JsonParserTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHoplinkSeRequestSet()
                            void JsonParserTest::testHoplinkSeRequestSet() {
                                std::shared_ptr<SeRequestSet> seRequestSet = SampleFactory::getASeRequestSet_ISO14443_4();
                                testSerializeDeserializeObj(seRequestSet, SeRequestSet::typeid);
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCompleteSeRequestSet()
                            void JsonParserTest::testCompleteSeRequestSet() {
                                std::shared_ptr<SeRequestSet> seRequestSet = SampleFactory::getCompleteRequestSet();
                                testSerializeDeserializeObj(seRequestSet, SeRequestSet::typeid);
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSeResponseSet()
                            void JsonParserTest::testSeResponseSet() {
                                std::shared_ptr<SeResponseSet> responseSet = SampleFactory::getCompleteResponseSet();
                                testSerializeDeserializeObj(responseSet, SeResponseSet::typeid);

                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSelectionRequest()
                            void JsonParserTest::testSelectionRequest() {
                                std::shared_ptr<SelectionRequest> selectionRequest = SampleFactory::getSelectionRequest();
                                testSerializeDeserializeObj(selectionRequest, SelectionRequest::typeid);

                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNotificationMode()
                            void JsonParserTest::testNotificationMode() {
                                ObservableReader::NotificationMode notificationMode = SampleFactory::getNotificationMode();
                                testSerializeDeserializeObj(notificationMode, ObservableReader::NotificationMode::typeid);
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimpleKeypleException()
                            void JsonParserTest::testSimpleKeypleException() {
                                std::shared_ptr<KeypleBaseException> exception = SampleFactory::getASimpleKeypleException();
                                testSerializeDeserializeObj(exception, KeypleBaseException::typeid);

                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStackedKeypleException()
                            void JsonParserTest::testStackedKeypleException() {
                                std::shared_ptr<KeypleBaseException> exception = SampleFactory::getAStackedKeypleException();
                                testSerializeDeserializeObj(exception, KeypleBaseException::typeid);

                            }

                            std::shared_ptr<void> JsonParserTest::testSerializeDeserializeObj(std::shared_ptr<void> obj, std::type_info objectClass) {
                                std::shared_ptr<Gson> gson = JsonParser::getGson();
                                std::string json = gson->toJson(obj);
                                logger->debug("json 1 : {}", json);
                                auto deserializeObj = gson->fromJson(json, objectClass);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                logger->debug("deserializeObj : {}", deserializeObj->toString());
                                std::string json2 = gson->toJson(deserializeObj);
                                logger->debug("json 2 : {}", json2);
                                assert(json == json2);
                                return deserializeObj;
                            }
                        }
                    }
                }
            }
        }
    }
}
