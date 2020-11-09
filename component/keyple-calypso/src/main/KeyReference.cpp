/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "KeyReference.h"

namespace keyple {
namespace calypso {

KeyReference::KeyReference(const uint8_t kif, const uint8_t kvc) : mKif(kif), mKvc(kvc) {}

uint8_t KeyReference::getKif() const
{
    return mKif;
}

uint8_t KeyReference::getKvc() const
{
    return mKvc;
}

}
}
