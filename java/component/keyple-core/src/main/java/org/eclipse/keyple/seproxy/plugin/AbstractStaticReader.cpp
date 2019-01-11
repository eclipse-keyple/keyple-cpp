#include "AbstractStaticReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    AbstractStaticReader::AbstractStaticReader(const std::string &pluginName, const std::string &readerName) : AbstractLocalReader(pluginName, readerName) {
                    }

                    void AbstractStaticReader::addObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) {
                        throw std::runtime_error("Abstract Static Reader does not support Observers, do not use this function");
                    }

                    void AbstractStaticReader::removeObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) {
                        throw std::runtime_error("Abstract Static Reader does not support Observers, do not use this function");
                    }
                }
            }
        }
    }
}
