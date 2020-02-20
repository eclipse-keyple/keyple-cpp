
#include "TagTest.h"

using namespace keyple::core::util;

        namespace keyple {
            namespace core {
                namespace util {

                    using namespace testing;
                    using namespace bertlv;

                    int TagTest::sample()
                    {
                        std::vector<char> local = {'A', 'B', 'C', 'D'};

                        Tag pLocal = Tag( local, 0 );
                        //pLocal.parse();
                        int iOffset = pLocal.getTagNumber();

                        return iOffset;
                    }
                }
            }
        }

TEST(TagTest, testA) 
{
    std::shared_ptr<TagTest> LocalTest = std::make_shared<TagTest>();
    int local = LocalTest->sample();

    ASSERT_NE( local, 0);
    ASSERT_EQ( local, 1);
}
