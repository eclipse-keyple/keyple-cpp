/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {

/**
    * indicate if the logical channel should be closed or not at the end of the request transmission
    */
enum class ChannelState {
    KEEP_OPEN,
    CLOSE_AFTER
};

}
}
}
}
}
