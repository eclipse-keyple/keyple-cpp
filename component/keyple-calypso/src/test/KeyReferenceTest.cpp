#include "KeyReferenceTest.h"

using namespace keyple::calypso;

namespace keyple {
namespace calypso {

using namespace testing;

int KeyReferenceTest::sample()
{
    std::shared_ptr<KeyReference> pLocal =
        std::make_shared<KeyReference>(static_cast<char>(32),
                                       static_cast<char>(0));
    return static_cast<int>(pLocal->getKvc());
}
}
}

TEST(KeyReferenceTest, testA)
{
    std::shared_ptr<KeyReferenceTest> LocalTest =
        std::make_shared<KeyReferenceTest>();
    int local = LocalTest->sample();

    ASSERT_NE(local, 1);
    ASSERT_EQ(local, 0);
}
