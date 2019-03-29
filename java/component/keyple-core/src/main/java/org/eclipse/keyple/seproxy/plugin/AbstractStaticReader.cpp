#include "AbstractStaticReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    AbstractStaticReader::AbstractStaticReader(const std::string &pluginName, const std::string &readerName) : AbstractLocalReader(pluginName, readerName) {
                    }

                    void AbstractStaticReader::startObservation() {
                    }

                    void AbstractStaticReader::stopObservation() {
                    }

                    void AbstractStaticReader::addObserver(std::shared_ptr<Observer> observer) {
                        throw std::make_shared<IllegalAccessError>("Abstract Static Reader does not support Observers, do not use this function");
                    }

                    void AbstractStaticReader::removeObserver(std::shared_ptr<Observer> observer) {
                        throw std::make_shared<IllegalAccessError>("Abstract Static Reader does not support Observers, do not use this function");
                    }
                }
            }
        }
    }
}
