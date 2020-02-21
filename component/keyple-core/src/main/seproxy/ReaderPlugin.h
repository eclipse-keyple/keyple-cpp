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

#include <set>
#include <string>

/* Common */
#include "Export.h"

/* Core */
#include "Configurable.h"
#include "KeypleReaderNotFoundException.h"
#include "Nameable.h"

/* Forward class declarations */
namespace keyple { namespace core { namespace seproxy { class SeReader; } } }

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::exception;
using namespace keyple::core::util;

/**
    * Card readers plugin interface.
    */
class EXPORT ReaderPlugin
: public virtual Nameable, public virtual Configurable {
public:
    /**
     *
     */
    ReaderPlugin() {}

    /**
     *
     */
    ReaderPlugin(const ReaderPlugin& o) : Nameable(), Configurable()
    {
        (void)o;
    }

    /**
     *
     */
    virtual ~ReaderPlugin() {}

    /**
     * Gets the list of names of all readers
     *
     * @return a list of String
     */
    virtual const std::set<std::string> getReaderNames() = 0;

    /**
     * Gets the readers.
     *
     * @return list of connected readers in this plugin, can be an empty list,
     *         can not be null;
     */
    virtual std::set<std::shared_ptr<SeReader>>& getReaders() = 0;

    /**
     * Gets the reader whose name is provided as an argument
     *
     * @param name of the reader
     * @return the SeReader object.
     * @throws KeypleReaderNotFoundException if the wanted reader is not found
     */
    virtual std::shared_ptr<SeReader> getReader(const std::string &name) = 0;
};

}
}
}
