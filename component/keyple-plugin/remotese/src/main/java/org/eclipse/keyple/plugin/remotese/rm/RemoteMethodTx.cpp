#include "RemoteMethodTx.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace rm {
                        using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                        using DtoSender = org::eclipse::keyple::plugin::remotese::transport::DtoSender;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        RemoteMethodTx<T>::ThreadAnonymousInnerClass::ThreadAnonymousInnerClass(std::shared_ptr<RemoteMethodTx<std::shared_ptr<T>>> outerInstance, std::shared_ptr<RemoteMethodTx> thisInstance) {
                            this->outerInstance = outerInstance;
                            this->thisInstance = thisInstance;
                        }

                        void RemoteMethodTx<T>::ThreadAnonymousInnerClass::run() {
                            try {
                                outerInstance->asyncGet(std::make_shared<RemoteMethodTxCallbackAnonymousInnerClass>(shared_from_this()));
                            }
                            catch (const KeypleRemoteException &e) {
                                logger->error("Exception while sending Dto", e);
                                thisInstance->remoteException = e;
                                outerInstance->lock->countDown();
                            }
                        }

                        RemoteMethodTx<T>::ThreadAnonymousInnerClass::RemoteMethodTxCallbackAnonymousInnerClass::RemoteMethodTxCallbackAnonymousInnerClass(std::shared_ptr<ThreadAnonymousInnerClass> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        void RemoteMethodTx<T>::ThreadAnonymousInnerClass::RemoteMethodTxCallbackAnonymousInnerClass::get(std::shared_ptr<T> response, std::shared_ptr<KeypleRemoteException> exception) {
                            logger::debug("release lock");
                            outerInstance->outerInstance.lock.countDown();
                        }
                    }
                }
            }
        }
    }
}
