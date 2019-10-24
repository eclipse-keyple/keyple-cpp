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

#include <stdexcept>

namespace keyple {
namespace common {

class Object {
public:
    /**
     *
     */
    Object() {}

    /**
     *
     */
    Object(const Object& o)
    {
        (void)o;
    }

    /**
     *
     */
    virtual ~Object() {}

    /**
     *
     */
	virtual bool equals(std::shared_ptr<void> o) = 0;

	/**
	 *
	 */
	virtual int hashCode() = 0;

protected:
	/**
	 *
	 */
	virtual void finalize() = 0;
};

}
}
