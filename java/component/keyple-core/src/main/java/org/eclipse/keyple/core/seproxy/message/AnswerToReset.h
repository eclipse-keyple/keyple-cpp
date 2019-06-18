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

#include <vector>
#include <memory>

/* Common*/
#include "Object.h"
#include "Export.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace message {

/**
    * AnswerToReset bytes wrapper.
    * <p>
    * (May be enhanced to provide analysis methods)
    */
class EXPORT AnswerToReset : public std::enable_shared_from_this<AnswerToReset>, public Object{
private:
    std::vector<char> atrBytes;

public:
    AnswerToReset(std::vector<char> &atrBytes);

    virtual ~AnswerToReset() { }

    std::vector<char> getBytes();

    bool equals(std::shared_ptr<void> o) override;

    int hashCode() override;

protected:
    void finalize() override;
};

}
}
}
}
}
}
