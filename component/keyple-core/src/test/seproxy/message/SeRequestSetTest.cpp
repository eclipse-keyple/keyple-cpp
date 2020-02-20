#include "SeRequestSetTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "SeRequestTest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
//                    import static org.junit.Assert.*;
                    using org::junit::Before;
                    using org::junit::Test;
                    using org::junit::runner::RunWith;
                    using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                    void SeRequestSetTest::setUp() throw(std::runtime_error) {
                        // init
                        sortedRequests = std::unordered_set<std::shared_ptr<SeRequest>>();

                        firstRequest = SeRequestTest::getSeRequestSample();
                        sortedRequests->add(firstRequest);

                        // init
                        seRequestSet = std::make_shared<SeRequestSet>(sortedRequests);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getRequests()
                    void SeRequestSetTest::getRequests() {
                        assertArrayEquals(sortedRequests->toArray(), seRequestSet->getRequests()->toArray());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getSingleRequest()
                    void SeRequestSetTest::getSingleRequest() {
                        seRequestSet = std::make_shared<SeRequestSet>(firstRequest);

                        // with only one element it works
                        assertEquals(firstRequest, seRequestSet->getSingleRequest());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void getSingleRequestFail()
                    void SeRequestSetTest::getSingleRequestFail() {
                        // put a second element
                        sortedRequests->add(SeRequestTest::getSeRequestSample());
                        seRequestSet = std::make_shared<SeRequestSet>(sortedRequests);
                        seRequestSet->getSingleRequest(); // raise exception because it works only with one element
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void toStringNull()
                    void SeRequestSetTest::toStringNull() {
                        seRequestSet = std::make_shared<SeRequestSet>(std::unordered_set<std::shared_ptr<SeRequest>>());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertNotNull(seRequestSet->toString());
                    }
                }
            }
        }
    }
}
