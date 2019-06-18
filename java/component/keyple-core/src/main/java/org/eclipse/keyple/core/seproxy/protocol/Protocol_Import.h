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

#ifndef KEYPLE_SEPROXY_PROTOCOL
#define KEYPLE_SEPROXY_PROTOCOL

#include <string>
#include <vector>

/* Common */
#include "Export.h"

/* Core */
#include "SeProtocol.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

class IMPORT Protocol final : public SeProtocol {

public:
    /**
     * Unspecified protocol
     */
    static Protocol ANY;

    /**
     * Constructor
     */
    Protocol(const std::string &name);

    /**
     *
     */
    enum class InnerEnum {
        ANY
    };

    /**
     *
     */
    const InnerEnum innerEnumValue;

    /**
     * Constructor
     */
    Protocol(const std::string &nameValue, InnerEnum innerEnum, const std::string &name);

    /**
     *
     */
    virtual std::string getName();

    /**
     *
     */
    bool operator==(const Protocol &other);

    /**
     *
     */
    bool operator!=(const Protocol &other);

    /**
     *
     */
    static std::vector<Protocol> values();

    /**
     *
     */
    int ordinal();

    /**
     *
     */
    std::string toString();

    /**
     *
     */
    static Protocol valueOf(const std::string &name);

private:
    /**
     *
     */
    static std::vector<Protocol> valueList;

    /**
     *
     */
    class StaticConstructor {
    public:
        /**
         *
         */
        StaticConstructor();
    };

    /**
     *
     */
    static StaticConstructor staticConstructor;

    /**
     *
     */
    const std::string nameValue;
    
    /**
     *
     */
    const int ordinalValue;

    /**
     *
     */
    static int nextOrdinal;
   
    /**
     * The protocol name
     */
    std::string name;
};

}
}
}
}
}
}

#endif /* KEYPLE_SEPROXY_PROTOCOL*/
