#include "ReaderEventTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace event_Renamed {
//                    import static org.junit.Assert.assertEquals;
//                    import static org.junit.Assert.assertNotNull;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testReaderEvent()
void ReaderEventTest::testReaderEvent()
{
    std::shared_ptr<ReaderEvent> event_Renamed = std::make_shared<ReaderEvent>(
        "plugin", "reader", ReaderEvent::EventType::IO_ERROR, nullptr);
    assertNotNull(event_Renamed);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetEvent()
void ReaderEventTest::testGetEvent()
{
    std::shared_ptr<ReaderEvent> event_Renamed = std::make_shared<ReaderEvent>(
        "plugin", "reader", ReaderEvent::EventType::IO_ERROR, nullptr);
    assertEquals(event_Renamed->getReaderName(), "reader");
    assertEquals(event_Renamed->getPluginName(), "plugin");
    assertEquals(event_Renamed->getEventType(),
                 ReaderEvent::EventType::IO_ERROR);
}
}
}
}
}
}
