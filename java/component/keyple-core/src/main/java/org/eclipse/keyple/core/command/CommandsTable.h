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

#include <string>
#include <typeinfo>


namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace command {

class CommandsTable {

public:
    virtual std::string getName() = 0;

    virtual char getInstructionByte() = 0;

    virtual const std::type_info& getCommandBuilderClass() = 0;

    virtual const std::type_info& getResponseParserClass() = 0;

};

}
}
}
}
}
