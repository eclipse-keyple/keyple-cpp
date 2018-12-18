#pragma once

#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace transaction { class PoTransaction; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace transaction { class CalypsoPo; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }

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
            namespace integration {
                namespace example {
                    namespace pc {
                        namespace calypso {

                            using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                            using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                            using SeReader = org::eclipse::keyple::seproxy::SeReader;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using namespace org::eclipse::keyple::seproxy::message;

                            class Demo_ValidationTransaction : public std::enable_shared_from_this<Demo_ValidationTransaction>, public ObservableReader::ReaderObserver {

                            private:
                                std::shared_ptr<SeReader> poReader, samReader;

                            public:
                                void update(std::shared_ptr<ReaderEvent> event_Renamed) override;

                                static std::vector<char> longToBytes(long long l);

                                static long long bytesToLong(std::vector<char> &b);

                                static int getCounterValueFromByteArray(std::vector<char> &inByteArray, int inCounterIndex);

                                static std::vector<char> getByteArrayFromCounterValue(int inCounterValue);

                                // Not optimized for online/remote operation
                            private:
                                void validateAuditC0(std::shared_ptr<PoTransaction> poTransaction) throw(KeypleReaderException);


                                // Optimised for online/remote operation
                                void validateClap(std::shared_ptr<CalypsoPo> detectedPO) throw(KeypleReaderException);


                                void detectAndHandlePO();

                                static const std::shared_ptr<void> waitForEnd;

                                static void main(std::vector<std::string> &args) throw(IOException, InterruptedException, KeypleBaseException);

                            };

                        }
                    }
                }
            }
        }
    }
}
