#include "AbstractPoCommandBuilderTest.h"
#include "AbstractPoCommandBuilder.h"

using namespace keyple::calypso::command::po;

    namespace keyple {
        namespace calypso {
            namespace command {
                namespace po {

                    void AbstractPoCommandBuilderTest::sample()
                    {
                        //std::shared_ptr<AbstractPoCommandBuilder<AbstractIso7816CommandBuilder>> pLocal = std::make_shared<AbstractPoCommandBuilder<AbstractIso7816CommandBuilder>>();
                        //std::string local = pLocal->getName();

                        //ASSERT_NE( local, "");
                    }
                }
            }
        }
    }


TEST(AbstractPoCommandBuilderTest, testA) 
{
    std::shared_ptr<AbstractPoCommandBuilderTest> LocalTest = std::make_shared<AbstractPoCommandBuilderTest>();
    LocalTest->sample();
}
