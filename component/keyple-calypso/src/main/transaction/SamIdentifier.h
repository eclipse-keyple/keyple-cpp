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

/* Calypso */
#include "KeypleCalypsoExport.h"
#include "SamRevision.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam;

/**
 * Holds the needed data to proceed a SAM selection.
 * <p>
 * SAM Revision (see {@link SamRevision})
 * <p>
 * Serial Number (may be a regular expression)
 * <p>
 * Group reference (key group reference)
 */
class SamIdentifier {
private:
    /**
     *
     */
    const SamRevision& samRevision;

    /**
     *
     */
    const std::string serialNumber;

    /**
     *
     */
    const std::string groupReference;

public:
    /**
     * Constructor for a SamIdentifier
     *
     * @param samRevision the SAM revision
     * @param serialNumber the SAM serial number as an hex string or a regular
     *        expression
     * @param groupReference the group reference string
     */
    SamIdentifier(const SamRevision& samRevision,
                  const std::string& serialNumber,
                  const std::string& groupReference);

    /**
     * @return the SAM revision
     */
    const SamRevision& getSamRevision() const;

    /**
     * @return the SAM serial number
     */
    const std::string& getSerialNumber() const;

    /**
     * @return the group reference
     */
    const std::string& getGroupReference() const;

    /**
     * Compare two SamIdentifiers with the following rules:
     * <ul>
     * <li>when the provided {@link SamIdentifier} is null the result is
     * true</li>
     * <li>when the provided {@link SamIdentifier} is not null
     * <ul>
     * <li>the AUTO revision matches any revision</li>
     * <li>if not null, the serial number is used as a regular expression to
     * check the current serial number</li>
     * <li>if not null the group reference is compared as a string</li>
     * </ul>
     * </li>
     * </ul>
     *
     * @param samIdentifier the {@link SamIdentifier} object to be compared to
     *        the current object
     * @return true if the identifier provided matches the current identifier
     */
    bool matches(const SamIdentifier* samIdentifier) const;
};

}
}
}
