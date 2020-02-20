
#include "TLVTest.h"

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util::bertlv;
using namespace keyple::core::util;
using namespace keyple::common;
using namespace keyple::calypso::command::po::parser;

std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> GetDataFciRespPars::STATUS_TABLE;

std::vector<int> const GetDataFciRespPars::BUFFER_SIZE_INDICATOR_TO_BUFFER_SIZE =
    std::vector<int> {0, 0, 0, 0, 0, 0, 215, 256, 304, 362, 430, 512, 608, 724, 861, 1024, 1217,
                      1448, 1722, 2048, 2435, 2896, 3444, 4096, 4870, 5792, 6888, 8192, 9741,
                      11585, 13777, 16384, 19483, 23170, 27554, 32768, 38967, 46340, 55108, 65536,
                      77935, 92681, 110217, 131072, 155871, 185363, 220435, 262144, 311743, 370727,
                      440871, 524288, 623487, 741455, 881743, 1048576};

const std::shared_ptr<Tag> GetDataFciRespPars::TAG_FCI_TEMPLATE =
    std::make_shared<Tag>(0x0F, Tag::APPLICATION, Tag::TagType::CONSTRUCTED);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_DF_NAME =
    std::make_shared<Tag>(0x04, Tag::CONTEXT, Tag::TagType::PRIMITIVE);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_FCI_PROPRIETARY_TEMPLATE =
    std::make_shared<Tag>(0x05, Tag::CONTEXT, Tag::TagType::CONSTRUCTED);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_FCI_ISSUER_DISCRETIONARY_DATA =
    std::make_shared<Tag>(0x0C, Tag::CONTEXT, Tag::TagType::CONSTRUCTED);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_APPLICATION_SERIAL_NUMBER =
    std::make_shared<Tag>(0x07, Tag::PRIVATE, Tag::TagType::PRIMITIVE);
const std::shared_ptr<Tag> GetDataFciRespPars::TAG_DISCRETIONARY_DATA =
    std::make_shared<Tag>(0x13, Tag::APPLICATION, Tag::TagType::PRIMITIVE);

        namespace keyple {
            namespace core {
                namespace util {

                    using namespace testing;
                    using namespace bertlv;

                    using Logger                = keyple::common::Logger;
                    using LoggerFactory         = keyple::common::LoggerFactory;

                    int TLVTest::sample()
                    {
                        std::vector<char> local = {'A', 'B', 'C', 'D'};
                        std::vector<char> apdu = {'A', 'B', 'C', 'D'};
                        std::vector<char> result = ByteArrayUtil::fromHex("9000");
                        std::shared_ptr<Tag> tag = std::make_shared<Tag>(local, 0);
                        std::shared_ptr<ApduResponse> selectApplicationResponse = std::make_shared<ApduResponse>(result, std::shared_ptr<std::set<int>>());

                        const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(TLVTest));

                        TLV pLocal = TLV( local );
                        int iOffset = pLocal.getPosition();
                        bool bLocal = pLocal.parse(tag, iOffset);
                        pLocal.toString();
                        local = pLocal.getValue();


                        const std::vector<char> response = selectApplicationResponse->getBytes();
                        std::shared_ptr<TLV> tlv;

                        /* check the command status to determine if the DF has been invalidated */
                        if (selectApplicationResponse->getStatusCode() == 0x6283) {
                            logger->debug("The response to the select application command status word indicates that" \
                                        "the DF has been invalidated\n");
                            isDfInvalidated_Renamed = true;
                        }

                        /* init TLV object with the raw data and extract the FCI Template */
                        logger->debug("response: %s\n", ByteArrayUtil::toHex(response));
                        std::vector<char> vec = response;
                        tlv = std::make_shared<TLV>(vec);

                        /* Get the FCI template */
//                        if (!tlv->parse(GetDataFciRespPars::TAG_FCI_TEMPLATE, 0)) {
                            logger->error("FCI parsing error: FCI template tag not found\n");
//                            return;
//                        }

                        /* Get the DF Name */
//                        if (!tlv->parse(TAG_DF_NAME, tlv->getPosition())) {
                            logger->error("FCI parsing error: DF name tag not found\n");
//                            return;
//                        }

                        dfName = tlv->getValue();
                        logger->debug("DF Name = %s\n", ByteArrayUtil::toHex(dfName));

                        /* Get the FCI Proprietary Template */
//                        if (!tlv->parse(TAG_FCI_PROPRIETARY_TEMPLATE, tlv->getPosition())) {
                            logger->error("FCI parsing error: FCI proprietary template tag not found\n");
//                            return;
//                        }

                        /* Get the FCI Issuer Discretionary Data */
//                        if (!tlv->parse(TAG_FCI_ISSUER_DISCRETIONARY_DATA, tlv->getPosition())) {
                            logger->error("FCI parsing error: FCI issuer discretionary data tag not found\n");
//                            return;
//                        }

                        /* Get the Application Serial Number */
//                        if (!tlv->parse(TAG_APPLICATION_SERIAL_NUMBER, tlv->getPosition())) {
                            logger->error("FCI parsing error: serial number tag not found\n");
//                            return;
//                        }

                        applicationSN = tlv->getValue();
                        logger->debug("Application Serial Number = %s\n", ByteArrayUtil::toHex(applicationSN));

                        /* Get the Discretionary Data */
//                        if (!tlv->parse(TAG_DISCRETIONARY_DATA, tlv->getPosition())) {
                            logger->error("FCI parsing error: discretionary data tag not found\n");
//                            return;
//                        }

                        std::vector<char> discretionaryData = tlv->getValue();

                        if (logger->isDebugEnabled()) {
                            logger->debug("Discretionary Data = %s\n", ByteArrayUtil::toHex(discretionaryData));
                        }

                        /*
                        * split discretionary data in as many individual startup information
                        */
                       if ( discretionaryData.size() != 0)
                       {
                        siBufferSizeIndicator = discretionaryData[0];
                        siPlatform = discretionaryData[1];
                        siApplicationType = discretionaryData[2];
                        siApplicationSubtype = discretionaryData[3];
                        siSoftwareIssuer = discretionaryData[4];
                        siSoftwareVersion = discretionaryData[5];
                        siSoftwareRevision = discretionaryData[6];
                        /* all 3 main fields were retrieved */
                        isValidCalypsoFCI_Renamed = true;
                       }


                        if ( bLocal ) iOffset = 0;
                        return iOffset;
                    }
                }
            }
        }

TEST(TLVTest, testA) 
{
    std::shared_ptr<TLVTest> LocalTest = std::make_shared<TLVTest>();
    int local = LocalTest->sample();

    ASSERT_NE( local, 1);
    ASSERT_EQ( local, 0);
}
