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

#pragma once

#include <vector>
#include <memory>

/* Common*/
#include "Object.h"
#include "LoggerFactory.h"

/* Core */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::common;

/**
 * AnswerToReset bytes wrapper.
 * <p>
 * (May be enhanced to provide analysis methods)
 */
class KEYPLECORE_API AnswerToReset {
public:
    /**
     *
     */
    explicit AnswerToReset(const std::vector<uint8_t>& atrBytes);

    /**
     *
     */
    const std::vector<uint8_t>& getBytes() const;

    /**
     *
     */
    bool operator==(const AnswerToReset& o) const;

    /**
     *
     */
    bool operator!=(const AnswerToReset& o) const;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const AnswerToReset& atr);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const std::shared_ptr<AnswerToReset>& atr);

private:
    /**
     *
     */
    const std::vector<uint8_t> atrBytes;
};

}
}
}
}
