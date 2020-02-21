
#include "ByteArrayUtilTest.h"

using namespace keyple::core::util;

namespace keyple {
namespace core {
namespace util {

using namespace testing;

int ByteArrayUtilTest::sample()
{
    std::vector<char> local = {'A', 'B', 'C', 'D'};
    std::string szIn        = "012345";
    int iOffset             = 4;

    ByteArrayUtil pLocal = ByteArrayUtil();
    local                = pLocal.fromHex(szIn);

    return iOffset;
}
}
}
}

TEST(ByteArrayUtilTest, testA)
{
    std::shared_ptr<ByteArrayUtilTest> LocalTest =
        std::make_shared<ByteArrayUtilTest>();
    int local = LocalTest->sample();

    ASSERT_NE(local, 0);
    ASSERT_EQ(local, 4);
}
