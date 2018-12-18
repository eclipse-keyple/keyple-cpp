#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"
#include "ReadDataStructure.h"
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "exceptionhelper.h"
#include <memory>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace command { class AbstractApduResponseParser; } } } }
namespace org { namespace eclipse { namespace keyple { namespace command { class StatusProperties; } } } }

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {

                            using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;

                            /**
                             * Read Records (00B2) response parser. See specs: Calypso / page 89 / 9.4.7 Read Records
                             */
                            class ReadRecordsRespPars final : public AbstractApduResponseParser {

                            private:
                                static const std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> STATUS_TABLE;

                                                        private:
                                                            class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                                            public:
                                                                StaticConstructor();
                                                            };

                                                        private:
                                                            static ReadRecordsRespPars::StaticConstructor staticConstructor;


                            protected:
                                std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> getStatusTable() override;

                                /** Type of data to parse: record data or counter, single or multiple */
                            private:
                                ReadDataStructure readDataStructure = static_cast<ReadDataStructure>(0);
                                /** Number of the first record read */
                                char recordNumber = 0;

                                /**
                                 * Instantiates a new ReadRecordsRespPars.
                                 *
                                 * @param recordNumber the record number
                                 * @param readDataStructure the type of content in the response to parse
                                 */
                            public:
                                ReadRecordsRespPars(char recordNumber, ReadDataStructure readDataStructure);

                                /**
                                 * Indicates whether the parser is associated with a counter file.
                                 * 
                                 * @return true or false
                                 */
                                bool isCounterFile();

                                /**
                                 * Parses the Apdu response as a data record (single or multiple), retrieves the records and
                                 * place it in an map.
                                 * <p>
                                 * An empty map is returned if no data is available.
                                 * 
                                 * @return a map of records
                                 * @exception IllegalStateException if the parser has not been initialized
                                 */
                                std::shared_ptr<SortedMap<Integer, std::vector<char>>> getRecords();

                                /**
                                 * Parses the Apdu response as a counter record (single or multiple), retrieves the counters
                                 * values and place it in an map indexed with the counter number.
                                 * <p>
                                 * An empty map is returned if no data is available.
                                 *
                                 * @return a map of counters
                                 * @exception IllegalStateException if the parser has not been initialized
                                 */
                                std::shared_ptr<SortedMap<Integer, Integer>> getCounters();

                                std::string toString() override;

protected:
                                std::shared_ptr<ReadRecordsRespPars> shared_from_this() {
                                    return std::static_pointer_cast<ReadRecordsRespPars>(org.eclipse.keyple.command.AbstractApduResponseParser::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
