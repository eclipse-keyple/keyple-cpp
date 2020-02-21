#include "AppendRecordRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "AppendRecordRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::po::parser::security;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {
using AbstractApduResponseParser =
    keyple::core::command::AbstractApduResponseParser;
using ApduResponse    = keyple::core::seproxy::message::ApduResponse;
using SeResponse      = keyple::core::seproxy::message::SeResponse;
using SeResponseSet   = keyple::core::seproxy::message::SeResponseSet;
using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
using ByteArrayUtils  = keyple::core::util::ByteArrayUtil;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void appendRecordRespPars()
void AppendRecordRespParsTest::appendRecordRespPars()
{
    std::vector<std::shared_ptr<ApduResponse>> responses;
    std::vector<char> ApduRequest = {90, 0};
    std::shared_ptr<ApduResponse> apduResponse =
        std::make_shared<ApduResponse>(ApduRequest, nullptr);
    responses.push_back(apduResponse);
    std::vector<char> cResp1 = ByteArrayUtils::fromHex("9000");
    std::shared_ptr<keyple::core::seproxy::message::AnswerToReset> atrBytes = {
        0};
    std::shared_ptr<SeResponseSet> seResponse =
        std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(
            true, true,
            std::make_shared<SelectionStatus>(
                atrBytes, std::make_shared<ApduResponse>(cResp1, nullptr),
                true),
            responses));

    std::shared_ptr<AbstractApduResponseParser> apduResponseParser =
        std::make_shared<AppendRecordRespPars>(apduResponse);
    apduResponseParser->setApduResponse(
        seResponse->getSingleResponse()->getApduResponses()[0]);
    ASSERT_EQ(ByteArrayUtils::toHex(ApduRequest),
              ByteArrayUtils::toHex(
                  apduResponseParser->getApduResponse()->getBytes()));
}
}
}
}
}
}
}

TEST(AppendRecordRespParsTest, testA)
{
    std::shared_ptr<AppendRecordRespParsTest> LocalTest =
        std::make_shared<AppendRecordRespParsTest>();
    LocalTest->appendRecordRespPars();
}
