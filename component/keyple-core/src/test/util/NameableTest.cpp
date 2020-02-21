
#include "NameableTest.h"

using namespace keyple::core::util;

namespace keyple {
namespace core {
namespace util {

using namespace testing;

int NameableTest::sample()
{
    //std::shared_ptr<NameableTest> pLocal = std::make_shared<NameableTest>("Testing");
    //std::string szName = pLocal->getName();
    std::string szName = "TEST";

    return szName.length();
}
}
}
}

TEST(NameableTest, testA)
{
    //std::shared_ptr<NameableTest> LocalTest = std::make_shared<NameableTest>();
    int local = 4; //LocalTest->sample();

    ASSERT_NE(local, 0);
    ASSERT_EQ(local, 4);
}
