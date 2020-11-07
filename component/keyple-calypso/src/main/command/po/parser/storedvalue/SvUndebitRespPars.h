/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "SvUndebitCmdBuild.h"
#include "SvLoadLogRecord.h"
#include "SvDebitLogRecord.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace storedvalue {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder::storedvalue;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

/**
 * SV Undebit (00BA) response parser. See specs: Calypso Stored Value balance (signed binaries'
 * coding based on the two's complement method)
 *
 * <p>
 * balance - 3 bytes signed binary - Integer from -8,388,608 to 8,388,607
 *
 * <pre>
    -8,388,608           %10000000.00000000.00000000
    -8,388,607           %10000000.00000000.00000001
    -8,388,606           %10000000.00000000.00000010

            -3           %11111111.11111111.11111101
            -2           %11111111.11111111.11111110
            -1           %11111111.11111111.11111111
             0           %00000000.00000000.00000000
             1           %00000000.00000000.00000001
             2           %00000000.00000000.00000010
             3           %00000000.00000000.00000011

     8,388,605           %01111111.11111111.11111101
     8,388,606           %01111111.11111111.11111110
     8,388,607           %01111111.11111111.11111111
 * </pre>
 *
 * amount - 2 bytes signed binary
 * <p>
 * amount for debit - Integer 0..32767 =&gt; for negative value
 *
 * <pre>
        -32767           %10000000.00000001
        -32766           %10000000.00000010
            -3           %11111111.11111101
            -2           %11111111.11111110
            -1           %11111111.11111111
             0           %00000000.00000000

    Notice: -32768 (%10000000.00000000) is not allowed.
 * </pre>
 */
class SvUndebitRespPars final : public AbstractPoResponseParser {
public:
    /**
     * Constructor to build a parser of the SvDebit command response.
     *
     * @param response response to parse
     * @param builder the reference to the builder that created this parser
     */
    SvUndebitRespPars(std::shared_ptr<ApduResponse> response,
                      SvUndebitCmdBuild* builder);

    /**
     * Gets the SV signature. <br>
     * The signature can be empty here in the case of a secure session where the transmission of the
     * signature is postponed until the end of the session.
     *
     * @return a byte array containing the signature
     */
    const std::vector<uint8_t> getSignatureLo() const;

    /**
     *
     */
    const std::string toString() const;

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable() const override;

private:
    /**
     *
     */
    static const std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;
};

}
}
}
}
}
}
