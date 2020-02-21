#include "SeResponseSetTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "SeResponseTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/AnswerToReset.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SelectionStatus.h"
#include "../../../../../../../main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace message {
//                    import static org.junit.Assert.*;
using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
using org::junit::Test;
using org::junit::runner::RunWith;
using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getSingleResponse() throws Exception
void SeResponseSetTest::getSingleResponse() throw(std::runtime_error)
{
    std::shared_ptr<SeResponseSet> set =
        std::make_shared<SeResponseSet>(SeResponseTest::getASeResponse());
    // assertArrayEquals(SeResponseTest.getASeResponse().getApduResponses().toArray(),
    // set.getSingleResponse().getApduResponses().toArray());
    assertEquals(SeResponseTest::getASeResponse(), set->getSingleResponse());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void getSingleResponseFail() throws Exception
void SeResponseSetTest::getSingleResponseFail() throw(std::runtime_error)
{
    std::vector<std::shared_ptr<SeResponse>> seResponses;
    seResponses.push_back(SeResponseTest::getASeResponse());
    seResponses.push_back(SeResponseTest::getASeResponse());
    std::shared_ptr<SeResponseSet> set =
        std::make_shared<SeResponseSet>(seResponses);

    set->getSingleResponse(); // throw exception
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getResponses() throws Exception
void SeResponseSetTest::getResponses() throw(std::runtime_error)
{
    std::vector<std::shared_ptr<SeResponse>> seResponses;
    seResponses.push_back(SeResponseTest::getASeResponse());
    seResponses.push_back(SeResponseTest::getASeResponse());
    std::shared_ptr<SeResponseSet> set =
        std::make_shared<SeResponseSet>(seResponses);
    assertArrayEquals(seResponses.toArray(), set->getResponses().toArray());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void toStringTest() throws Exception
void SeResponseSetTest::toStringTest() throw(std::runtime_error)
{
    std::shared_ptr<SeResponse> emptySeR = std::make_shared<SeResponse>(
        true,
        std::make_shared<SelectionStatus>(
            std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3BFF00")),
            nullptr, true),
        nullptr);
    std::shared_ptr<SeResponseSet> set =
        std::make_shared<SeResponseSet>(emptySeR);
    //JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    assertNotNull(set->toString());
}
}
}
}
}
}
