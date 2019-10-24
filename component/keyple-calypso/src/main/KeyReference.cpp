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

#include "KeyReference.h"

namespace keyple {
namespace calypso {

KeyReference::KeyReference(char kif, char kvc) : kif(kif), kvc(kvc)
{
}

char KeyReference::getKif()
{
        return kif;
}

char KeyReference::getKvc()
{
        return kvc;
}
}
}
