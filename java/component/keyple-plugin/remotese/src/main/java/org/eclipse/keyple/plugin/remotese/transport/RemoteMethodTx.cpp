#include "RemoteMethodTx.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        RemoteMethodTx<T>::ThreadAnonymousInnerClass::ThreadAnonymousInnerClass(std::shared_ptr<RemoteMethodTx<std::shared_ptr<T>>> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        void RemoteMethodTx<T>::ThreadAnonymousInnerClass::run() {
                            try {
                                outerInstance->asyncGet(std::make_shared<RemoteMethodTxCallbackAnonymousInnerClass>(shared_from_this()));
                            }
                            catch (const KeypleRemoteException &e) {
                                logger->error("Exception while sending Dto");
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
