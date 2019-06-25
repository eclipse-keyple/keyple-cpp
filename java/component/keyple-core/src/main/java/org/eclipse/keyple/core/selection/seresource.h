/********************************************************************************
* Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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

#include <memory>
#include <type_traits>

#include "SeReader.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using SeReader = org::eclipse::keyple::core::seproxy::SeReader;

/**
    * The SeResource class groups a AbstractMatchingSe and its associated SeReader
    */
template<typename T>
class SeResource : public std::enable_shared_from_this<SeResource<T>> {
    static_assert(std::is_base_of<AbstractMatchingSe, T>::value, "T must inherit from AbstractMatchingSe");

private:
    const std::shared_ptr<SeReader> seReader;
    const T matchingSe;

    /**
        * Constructor
        * 
        * @param seReader the {@link SeReader} with which the SE is communicating
        * @param matchingSe the {@link AbstractMatchingSe} information structure
        */
protected:
    SeResource(std::shared_ptr<SeReader> seReader, T matchingSe) : seReader(seReader), matchingSe(matchingSe) {
    }

    /**
        * @return the current {@link SeReader} for this SE
        */
public:
    virtual std::shared_ptr<SeReader> getSeReader() {
        return seReader;
    }

    /**
        * @return the {@link AbstractMatchingSe}
        */
    virtual T getMatchingSe() {
        return matchingSe;
    }
};

}
}
}
}
}
