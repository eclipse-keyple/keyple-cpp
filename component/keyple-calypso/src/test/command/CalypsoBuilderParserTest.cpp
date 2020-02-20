#include "CalypsoBuilderParserTest.h"


using namespace keyple::calypso::command;

        namespace keyple {
            namespace calypso {
                namespace command {   

                    void CalypsoBuilderParserTest::sample()
                    {
                        //std::shared_ptr<CalypsoBuilderParser> pLocal = std::make_shared<CalypsoBuilderParser>();

                        //pLocal->getKvc();
                    }
                }
            }
        }


TEST(CalypsoBuilderParserTest, testA) 
{
    std::shared_ptr<CalypsoBuilderParserTest> LocalTest = std::make_shared<CalypsoBuilderParserTest>();
    LocalTest->sample();
}
