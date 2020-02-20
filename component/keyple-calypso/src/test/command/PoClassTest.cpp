#include "PoClassTest.h"


using namespace keyple::calypso::command;

        namespace keyple {
            namespace calypso {
                namespace command {
                    using namespace testing;

                    int PoClassTest::sample()
                    {
                        std::shared_ptr<PoClass> pLocal = std::make_shared<PoClass>( "TEST", PoClass::InnerEnum::ISO, 'X');
                        return (int)pLocal->getValue();
                    }
                }
            }
        }


TEST(PoClassTest, testA) 
{
    std::shared_ptr<PoClassTest> LocalTest = std::make_shared<PoClassTest>();
    int local = LocalTest->sample();

    ASSERT_NE( local, 1);
    ASSERT_EQ( local, 'X');
}
