#include "BaseStubTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using org::junit::Assert;
                    using org::junit::Rule;
                    using org::junit::rules::TestName;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> BaseStubTest::logger = org::slf4j::LoggerFactory::getLogger(BaseStubTest::typeid);

                    void BaseStubTest::setUp() throw(std::runtime_error) {
                        logger->info("------------------------------");
                        logger->info("Test {}", name->getMethodName());
                        logger->info("------------------------------");

                        logger->info("setUp, assert stubplugin is empty");
                        stubPlugin = StubPlugin::getInstance(); // singleton

                        logger->info("Stubplugin readers size {}", stubPlugin->getReaders()->size());
                        Assert::assertEquals(0, stubPlugin->getReaders()->size());

                        logger->info("Stubplugin observers size {}", stubPlugin->countObservers());
                        Assert::assertEquals(0, stubPlugin->countObservers());

                        // add a sleep to play with thread monitor timeout
                        delay(100);

                    }

                    void BaseStubTest::tearDown() throw(InterruptedException, KeypleReaderException) {
                        logger->info("---------");
                        logger->info("TearDown ");
                        logger->info("---------");

                        stubPlugin = StubPlugin::getInstance(); // singleton

                        stubPlugin->unplugStubReaders(stubPlugin->getReaderNames(), true);
                        /*
                         * for (AbstractObservableReader reader : stubPlugin.getReaders()) {
                         * logger.info("Stubplugin unplugStubReader {}", reader.getName());
                         * stubPlugin.unplugStubReader(reader.getName(), true); Thread.sleep(100); //
                         * logger.debug("Stubplugin readers size {}", stubPlugin.getReaders().size()); }
                         */
                        stubPlugin->clearObservers();

                    }
                }
            }
        }
    }
}
