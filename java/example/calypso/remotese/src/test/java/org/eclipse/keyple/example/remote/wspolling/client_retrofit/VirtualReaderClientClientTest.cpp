#include "VirtualReaderClientClientTest.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/example/remote/wspolling/client_retrofit/WsPRetrofitClient.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/example/remote/wspolling/client_retrofit/WsPRetrofitClientImpl.h"
#include "../../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleDto.h"
#include "../../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/RemoteMethod.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace client_retrofit {
//                            import static org.junit.Assert.assertEquals;
//                            import static org.junit.Assert.assertFalse;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using RemoteMethod = org::eclipse::keyple::plugin::remotese::transport::RemoteMethod;
                            using org::junit::Before;
                            using retrofit2::Response;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void seTup()
                            void VirtualReaderClientClientTest::seTup() {
                            }

                            void VirtualReaderClientClientTest::testPolling() throw(IOException) {
                                std::shared_ptr<WsPRetrofitClient> rseClient = WsPRetrofitClientImpl::getRseAPIClient(BASE_URL);
                                std::shared_ptr<Response<std::shared_ptr<KeypleDto>>> kdto = rseClient->getPolling("clientNodeId")->execute();

                            }

                            void VirtualReaderClientClientTest::testPostDto() throw(IOException) {

                                std::shared_ptr<KeypleDto> dtoConnect = std::make_shared<KeypleDto>(RemoteMethod::READER_CONNECT.getName(), "{nativeReaderName:test, clientNodeId:testnode1}", true);

                                std::shared_ptr<WsPRetrofitClient> rseClient = WsPRetrofitClientImpl::getRseAPIClient(BASE_URL);
                                std::shared_ptr<Response<std::shared_ptr<KeypleDto>>> resp = rseClient->postDto(dtoConnect)->execute();


                                assertEquals(200, resp->code());
                                assertEquals(RemoteMethod::READER_CONNECT.getName(), resp->body().getAction());
                                assertFalse(resp->body().isRequest());

                            }
                        }
                    }
                }
            }
        }
    }
}
