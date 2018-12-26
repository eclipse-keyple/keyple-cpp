#pragma once

#include "KeypleRemoteException.h"
#include "RemoteMethodTxCallback.h"
#include "DtoSender.h"
#include "KeypleDto.h"
#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleRemoteException; } } } } } }

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
            namespace plugin {
                namespace remotese {
                    namespace transport {

                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Abstract class to implement blocking and non blocking call to a Remote Method that sends a
                         * response
                         * 
                         * @param <T> : type of the response
                         */
                        template<typename T>
                        class RemoteMethodTx : public std::enable_shared_from_this<RemoteMethodTx> {

                        private:
//JAVA TO C++ CONVERTER TODO TASK: Native C++ does not allow initialization of static non-const/integral fields in their declarations - choose the conversion option for separate .h and .cpp files:
                            static const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(RemoteMethodTx::typeid);
                        protected:
                            std::string sessionId;
                            std::string nativeReaderName;
                            std::string virtualReaderName;
                            std::string clientNodeId;

                            // response
                        private:
                            T response;

                            // exception thrown if any
                            std::shared_ptr<KeypleRemoteException> remoteException;

                            // blocking mecanism
                            std::shared_ptr<CountDownLatch> lock;
                            std::shared_ptr<RemoteMethodTxCallback<T>> callback;

                            std::shared_ptr<DtoSender> sender;

                        protected:
                            RemoteMethodTx(const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId) {
                                this->sessionId = sessionId;
                                this->nativeReaderName = nativeReaderName;
                                this->virtualReaderName = virtualReaderName;
                                this->clientNodeId = clientNodeId;
                            }


                        public:
                            virtual void setDto(std::shared_ptr<DtoSender> sender) {
                                this->sender = sender;
                            }

                            /**
                             * Internal method to set manually the keypleDto response To be called by the tx manager
                             */
                            virtual T parseResponse(std::shared_ptr<KeypleDto> keypleDto) = 0;


                            /**
                             * Non blocking method to get results from the remote method call
                             * 
                             * @param callback
                             */
                            void asyncGet(std::shared_ptr<RemoteMethodTxCallback<T>> callback) throw(KeypleRemoteException) {
                                this->callback = callback;
                                sender->sendDTO(this->dto());
                            }


                            /**
                             * Blocking method to get results from the remote method call. To be called by the client
                             * 
                             * @return T :
                             * @throws KeypleRemoteException
                             */
                            T get() throw(KeypleRemoteException) {
                                logger->debug("Blocking Get {}");

                                std::shared_ptr<Thread> asyncGet = std::make_shared<ThreadAnonymousInnerClass>(shared_from_this());

                                try {

                                    lock = std::make_shared<CountDownLatch>(1);
                                    asyncGet->start();
                                    lock->await();
                                    logger->debug("tread unlock in RemoteMethodTx");
                                    if (this->remoteException != nullptr) {
                                        throw remoteException;
                                    }
                                    else {
                                        return response;
                                    }

                                }
                                catch (const InterruptedException &e) {
                                    e->printStackTrace();
                                    throw std::make_shared<IllegalStateException>("Thread locking in blocking transmitSet has encountered an exception", e);
                                }
                            }

                        private:
                            class ThreadAnonymousInnerClass : public Thread {
                            private:
                                std::shared_ptr<RemoteMethodTx<std::shared_ptr<T>>> outerInstance;

                            public:
                                ThreadAnonymousInnerClass(std::shared_ptr<RemoteMethodTx<std::shared_ptr<T>>> outerInstance);

                                void run();

                            private:
                                class RemoteMethodTxCallbackAnonymousInnerClass : public std::enable_shared_from_this<RemoteMethodTxCallbackAnonymousInnerClass>, public RemoteMethodTxCallback<std::shared_ptr<T>> {
                                private:
                                    std::shared_ptr<ThreadAnonymousInnerClass> outerInstance;

                                public:
                                    RemoteMethodTxCallbackAnonymousInnerClass(std::shared_ptr<ThreadAnonymousInnerClass> outerInstance);

                                    void get(std::shared_ptr<T> response, std::shared_ptr<KeypleRemoteException> exception) override;
                                };

protected:
                                std::shared_ptr<ThreadAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<ThreadAnonymousInnerClass>(Thread::shared_from_this());
                                }
                            };

                            /**
                             * Process the response contains in the keypleDto Response
                             * 
                             * @param keypleDto
                             */
                        public:
                            virtual void asyncSetResponse(std::shared_ptr<KeypleDto> keypleDto) {
                                try {
                                    this->response = parseResponse(keypleDto);
                                    this->callback->get(response, nullptr);
                                }
                                catch (const KeypleRemoteException &e) {
                                    this->remoteException = e;
                                    this->callback->get(nullptr, e);
                                }
                            }

                            /**
                             * Generates a Request Dto for this Remote Method call
                             * 
                             * @return
                             */
                            virtual std::shared_ptr<KeypleDto> dto() = 0;


                        };

                    }
                }
            }
        }
    }
}
