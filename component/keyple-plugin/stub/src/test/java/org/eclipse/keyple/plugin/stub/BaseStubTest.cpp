#include "BaseStubTest.h"
#include "StubPlugin.h"
#include "KeypleReaderException.h"

        namespace keyple {
            namespace plugin {
                namespace stub {
                    using Logger                = keyple::common::Logger;
                    using LoggerFactory         = keyple::common::LoggerFactory;
                    using namespace std::chrono; // nanoseconds, system_clock, seconds

                    const std::shared_ptr<Logger> BaseStubTest::logger = LoggerFactory::getLogger( typeid(BaseStubTest) );

                    void BaseStubTest::setUp() 
                    {
                        logger->info("------------------------------");
                        logger->info("Test {}", typeid(name).name() );//->getMethodName());
                        logger->info("------------------------------");

                        logger->info("setUp, assert stubplugin is empty");
                        stubPlugin = std::make_shared<StubPlugin>( StubPlugin::getInstance() );

                        logger->info("Stubplugin readers size {}", stubPlugin->getReaders()->size());
                        ASSERT_EQ(0, stubPlugin->getReaders()->size());

                        logger->info("Stubplugin observers size {}", stubPlugin->countObservers());
                        ASSERT_EQ(0, stubPlugin->countObservers());

                        // add a sleep to play with thread monitor timeout
                        sleep(1);
                    }

                    void BaseStubTest::tearDown() 
                    {
                        logger->info("---------");
                        logger->info("TearDown ");
                        logger->info("---------");

                        stubPlugin = std::make_shared<StubPlugin>( StubPlugin::getInstance() );

                        stubPlugin->unplugStubReaders(stubPlugin->getReaderNames(), true);
                        
                        //AbstractObservableReader reader = stubPlugin->getReaders;
                        {
                           //logger->info("Stubplugin unplugStubReader {}", reader.getName());
                           //stubPlugin->unplugStubReader(reader.getName(), true); 
                           sleep(100);
                           logger->debug("Stubplugin readers size {}", stubPlugin->getReaders()->size()); 
                        }
                        stubPlugin->clearObservers();
                    }

                    void BaseStubTest::TestBoby()
                    {                       
                    }
                }
            }
        }
