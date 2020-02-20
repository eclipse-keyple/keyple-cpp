
#include "CountDownLatchTest.h"

using namespace keyple::core::util;

        namespace keyple {
            namespace core {
                namespace util {

                    using namespace testing;

                    int CountDownLatchTest::sample( unsigned int iValue )
                    {
                        int iResult;

                        std::shared_ptr<CountDownLatch> pLocal = std::make_shared<CountDownLatch>(iValue);
                        pLocal->countDown();
                        iResult = pLocal->getCount();

                        return iResult;
                    }
                }
            }
        }

TEST(CountDownLatchTest, testA) 
{
    std::shared_ptr<CountDownLatchTest> LocalTest = std::make_shared<CountDownLatchTest>();
    int local = LocalTest->sample( 5 );

    ASSERT_NE( local, 5);
    ASSERT_EQ( local, 4);
}
