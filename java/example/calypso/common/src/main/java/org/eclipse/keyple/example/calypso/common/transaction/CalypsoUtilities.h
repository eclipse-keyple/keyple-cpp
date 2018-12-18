#pragma once

#include "../../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"
#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeProxyService; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace transaction { class PoTransaction; } } } } }

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
                            using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                            using SeReader = org::eclipse::keyple::seproxy::SeReader;
                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;

                            class CalypsoUtilities : public std::enable_shared_from_this<CalypsoUtilities> {
                            private:
                                static std::shared_ptr<Properties> properties;

                                                        private:
                                                            class StaticConstructor : public std::enable_shared_from_this<StaticConstructor> {
                                                            public:
                                                                StaticConstructor();
                                                            };

                                                        private:
                                                            static CalypsoUtilities::StaticConstructor staticConstructor;


                                /**
                                 * Get the default reader for PO communications
                                 * 
                                 * @param seProxyService the current ProxyService
                                 * @return a SeReader object
                                 * @throws KeypleBaseException if an error occurred
                                 */
                            public:
                                static std::shared_ptr<SeReader> getDefaultPoReader(std::shared_ptr<SeProxyService> seProxyService) throw(KeypleBaseException);

                                /**
                                 * Get the default reader for SAM communications
                                 * 
                                 * @param seProxyService the current ProxyService
                                 * @return a SeReader object
                                 * @throws KeypleBaseException if an error occurred
                                 */
                                static std::shared_ptr<SeReader> getDefaultSamReader(std::shared_ptr<SeProxyService> seProxyService) throw(KeypleBaseException);

                                static std::shared_ptr<EnumMap<PoTransaction::SamSettings, Byte>> getSamSettings();

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

                                /**
                                 * Check SAM presence and consistency
                                 * <p>
                                 * Throw an exception if the expected SAM is not available
                                 *
                                 * @param samReader the SAM reader
                                 */
                            public:
                                static void checkSamAndOpenChannel(std::shared_ptr<SeReader> samReader);
                            };

                        }
                    }
                }
            }
        }
    }
}
