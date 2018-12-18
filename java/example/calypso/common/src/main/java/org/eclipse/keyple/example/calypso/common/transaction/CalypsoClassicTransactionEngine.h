#pragma once

#include "../../../../../../../../../../../../generic/common/src/main/java/org/eclipse/keyple/example/generic/common/AbstractReaderObserverEngine.h"
#include "../../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace transaction { class PoTransaction; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }

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
            namespace example {
                namespace calypso {
                    namespace common {
                        namespace transaction {

                            using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                            using AbstractReaderObserverEngine = org::eclipse::keyple::example::generic_Renamed::common::AbstractReaderObserverEngine;
                            using SeReader = org::eclipse::keyple::seproxy::SeReader;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using namespace org::eclipse::keyple::transaction;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

                            /**
                             * This Calypso demonstration code consists in:
                             *
                             * <ol>
                             * <li>Setting up a two-reader configuration and adding an observer method ({@link #update update})
                             * <li>Starting a card operation when a PO presence is notified
                             * ({@link #processSeMatch(SelectionResponse)} operateSeTransaction})
                             * <li>Opening a logical channel with the SAM (C1 SAM is expected) see
                             * ({@link CalypsoClassicInfo#SAM_C1_ATR_REGEX SAM_C1_ATR_REGEX})
                             * <li>Attempting to open a logical channel with the PO with 3 options:
                             * <ul>
                             * <li>Selecting with a fake AID (1)
                             * <li>Selecting with the Calypso AID and reading the event log file
                             * <li>Selecting with a fake AID (2)
                             * </ul>
                             * <li>Display {@link SelectionResponse} data
                             * <li>If the Calypso selection succeeded, do a Calypso transaction
                             * ({doCalypsoReadWriteTransaction(PoTransaction, ApduResponse, boolean)}
                             * doCalypsoReadWriteTransaction}).
                             * </ol>
                             *
                             * <p>
                             * The Calypso transactions demonstrated here shows the Keyple API in use with Calypso SE (PO and
                             * SAM).
                             *
                             * <p>
                             * Read the doc of each methods for further details.
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("unused") public class CalypsoClassicTransactionEngine extends org.eclipse.keyple.example.generic.common.AbstractReaderObserverEngine
                            class CalypsoClassicTransactionEngine : public AbstractReaderObserverEngine {
                            private:
                                static std::shared_ptr<Logger> logger;

                                /* define the SAM parameters to provide when creating PoTransaction */
                                static const std::shared_ptr<EnumMap<PoTransaction::SamSettings, Byte>> samSetting;

                            private:
                                class EnumMapAnonymousInnerClass : public EnumMap<PoTransaction::SamSettings, Byte> {
                                public:
                                    EnumMapAnonymousInnerClass();

                            //        {
                            //            put(PoTransaction.SamSettings.SAM_DEFAULT_KIF_PERSO, PoTransaction.DEFAULT_KIF_PERSO);
                            //            put(PoTransaction.SamSettings.SAM_DEFAULT_KIF_LOAD, PoTransaction.DEFAULT_KIF_LOAD);
                            //            put(PoTransaction.SamSettings.SAM_DEFAULT_KIF_DEBIT, PoTransaction.DEFAULT_KIF_DEBIT);
                            //            put(PoTransaction.SamSettings.SAM_DEFAULT_KEY_RECORD_NUMBER, PoTransaction.DEFAULT_KEY_RECORD_NUMER);
                            //        }

protected:
                                    std::shared_ptr<EnumMapAnonymousInnerClass> shared_from_this() {
                                        return std::static_pointer_cast<EnumMapAnonymousInnerClass>(java.util.EnumMap<org.eclipse.keyple.calypso.transaction.PoTransaction.SamSettings, Byte>::shared_from_this());
                                    }
                                };

                            private:
                                std::shared_ptr<SeReader> poReader, samReader;

                                std::shared_ptr<SeSelection> seSelection;

                                bool samChannelOpen = false;

                                /* Constructor */
                            public:
                                CalypsoClassicTransactionEngine();

                                /* Assign readers to the transaction engine */
                                virtual void setReaders(std::shared_ptr<SeReader> poReader, std::shared_ptr<SeReader> samReader);

                                /**
                                 * Do a Calypso transaction
                                 * <p>
                                 * Nominal case (the previous transaction was ratified):
                                 * <ul>
                                 * <li>Process opening
                                 * <ul>
                                 * <li>Reading the event log file</li>
                                 * <li>Reading the contract list</li>
                                 * </ul>
                                 * </li>
                                 * <li>Process PO commands
                                 * <ul>
                                 * <li>Reading the 4 contracts</li>
                                 * </ul>
                                 * </li>
                                 * <li>Process closing
                                 * <ul>
                                 * <li>A new record is appended to the event log file</li>
                                 * <li>The session is closed in CONTACTLESS_MODE (a ratification command is sent)</li>
                                 * </ul>
                                 * </li>
                                 * </ul>
                                 * <p>
                                 * Alternate case (the previous transaction was not ratified):
                                 * <ul>
                                 * <li>Process opening
                                 * <ul>
                                 * <li>Reading the event log file</li>
                                 * <li>Reading the contract list</li>
                                 * </ul>
                                 * </li>
                                 * <li>Process closing
                                 * <ul>
                                 * <li>The session is closed in CONTACTLESS_MODE (a ratification command is sent)</li>
                                 * </ul>
                                 * </li>
                                 * </ul>
                                 * <p>
                                 * The PO logical channel is kept open or closed according to the closeSeChannel flag
                                 *
                                 * @param poTransaction PoTransaction object
                                 * @param closeSeChannel flag to ask or not the channel closing at the end of the transaction
                                 * @throws KeypleReaderException reader exception (defined as public for purposes of javadoc)
                                 */
                                virtual void doCalypsoReadWriteTransaction(std::shared_ptr<PoTransaction> poTransaction, bool closeSeChannel) throw(KeypleReaderException);

                                virtual std::shared_ptr<SelectionRequest> preparePoSelection();

                                /**
                                 * Do the PO selection and possibly go on with Calypso transactions.
                                 */
                                void processSeMatch(std::shared_ptr<SelectionResponse> selectionResponse) override;

                                void processSeInsertion() override;

                                void processSeRemoval() override;

                                void processUnexpectedSeRemoval() override;

protected:
                                std::shared_ptr<CalypsoClassicTransactionEngine> shared_from_this() {
                                    return std::static_pointer_cast<CalypsoClassicTransactionEngine>(org.eclipse.keyple.example.generic_Renamed.common.AbstractReaderObserverEngine::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
