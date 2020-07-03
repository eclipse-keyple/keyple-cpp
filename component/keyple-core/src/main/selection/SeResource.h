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

#include <memory>
#include <type_traits>

#include "AbstractMatchingSe.h"
#include "SeReader.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::seproxy;

/**
 * The SeResource class groups a AbstractMatchingSe and its associated
 * SeReader<br>
 * TODO This class should be final.<br>
 * For the moment, it is not, awaiting the rewriting of the code of a client
 * class in Keyple-Calypso.
 */
template <typename T>
class SeResource : public std::enable_shared_from_this<SeResource<T>> {
    static_assert(std::is_base_of<AbstractMatchingSe, T>::value,
                  "T must inherit from AbstractMatchingSe");
public:
    /**
     * Constructor
     *
     * @param seReader the {@link SeReader} with which the SE is communicating
     * @param matchingSe the {@link AbstractMatchingSe} information structure
     */
    SeResource(std::shared_ptr<SeReader> seReader,
               std::shared_ptr<T> matchingSe)
    : seReader(seReader), matchingSe(matchingSe) {}

    /**
     *
     */
    virtual ~SeResource() {}

    /**
     * @return the current {@link SeReader} for this SE
     */
    virtual std::shared_ptr<SeReader> getSeReader()
    {
        return seReader;
    }

    /**
     * @return the {@link AbstractMatchingSe}
     */
    virtual std::shared_ptr<T> getMatchingSe()
    {
        return matchingSe;
    }

private:
    /**
     *
     */
    const std::shared_ptr<SeReader> seReader;

    /**
     *
     */
    const std::shared_ptr<T> matchingSe;
};

}
}
}
