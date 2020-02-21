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

#include <stdexcept>
#include <memory>

#include "Object.h"
#include "AnswerToReset.h"

/* Forward class declarations */
namespace keyple { namespace core { namespace seproxy { namespace message {
    class ApduResponse; } } } }

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

class SelectionStatus
: public std::enable_shared_from_this<SelectionStatus>, public Object {
public:
    /**
     *
     */
    SelectionStatus(std::shared_ptr<AnswerToReset> atr,
                    std::shared_ptr<ApduResponse> fci, bool hasMatched);

    /**
     *
     */
    virtual ~SelectionStatus() { }

    /**
     *
     */
    virtual std::shared_ptr<AnswerToReset> getAtr();

    /**
     *
     */
    virtual std::shared_ptr<ApduResponse> getFci();

    /**
     *
     */
    virtual bool hasMatched();

    /**
     *
     */
    bool equals(std::shared_ptr<void> o) override;

    /**
     *
     */
    int hashCode() override;

protected:
    /**
     *
     */
    void finalize() override;

private:
    /**
     *
     */
    const std::shared_ptr<AnswerToReset> atr;

    /**
     *
     */
    const std::shared_ptr<ApduResponse> fci;

    /**
     *
     */
    const bool isMatching;
};

}
}
}
}
