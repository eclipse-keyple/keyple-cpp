/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "ReaderEventTest.h"
#include "ReaderEvent.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace event_Renamed {

void ReaderEventTest::testReaderEvent()
{
    std::shared_ptr<ReaderEvent> event_Renamed = std::make_shared<ReaderEvent>(
        "plugin", "reader", ReaderEvent::EventType::IO_ERROR, nullptr);
    assertNotNull(event_Renamed);
}

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
