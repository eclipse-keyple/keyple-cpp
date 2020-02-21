#include "DigestUpdateRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "DigestUpdateRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::sam::parser;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
using DigestUpdateRespPars =
    keyple::calypso::command::sam::parser::security::DigestUpdateRespPars;
using AbstractApduResponseParser =
    keyple::core::command::AbstractApduResponseParser;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SeResponseSet   = keyple::core::seproxy::message::SeResponseSet;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void digestUpdateRespPars()
void DigestUpdateRespParsTest::digestUpdateRespPars()
{
    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::vector<char> cResp0 = std::vector<char>{90, 0};
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(cResp0, nullptr);
    responses.push_back(apduResponse);
    std::vector<char> cResp1 = ByteArrayUtils::fromHex("9000");
    std::shared_ptr<SeResponseSet> seResponse =
        std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                nullptr, std::make_shared<ApduResponse>(cResp1, nullptr), true),
            responses));

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<DigestUpdateRespPars>(
            seResponse->getSingleResponse()->getApduResponses()[0]);
    std::vector<char> responseActual =
        apduResponseParser->getApduResponse()->getBytes();
    std::vector<char> cResp = std::vector<char>{90, 0};
    ASSERT_EQ(ByteArrayUtils::toHex(cResp),
              ByteArrayUtils::toHex(responseActual));
}
}
}
}
}
}

TEST(DigestUpdateRespParsTest, testA)
{
    std::shared_ptr<DigestUpdateRespParsTest> LocalTest =
        std::make_shared<DigestUpdateRespParsTest>();
    LocalTest->digestUpdateRespPars();
}
