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

#include "CalypsoPo.h"
#include "PoResource.h"
#include "SeReader.h"
#include "SeResource.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::selection;
using namespace keyple::core::seproxy;

PoResource::PoResource(std::shared_ptr<SeReader> seReader,
                       std::shared_ptr<CalypsoPo> calypsoPo)
: SeResource<CalypsoPo>(seReader, calypsoPo)
{
}

}
}
}
