#include "SendableInSessionTest.h"


using namespace keyple::calypso::command;

        namespace keyple {
            namespace calypso {
                namespace command {
                    using namespace testing;

                    void SendableInSessionTest::sample()
                    {
                        //std::shared_ptr<SendableInSession> pLocal = std::make_shared<SendableInSession>();
                    }
                }
            }
        }


TEST(SendableInSessionTest, testA) 
{
    std::shared_ptr<SendableInSessionTest> LocalTest = std::make_shared<SendableInSessionTest>();
    LocalTest->sample();
}
