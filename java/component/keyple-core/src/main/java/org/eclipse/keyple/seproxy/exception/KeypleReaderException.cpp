#include "KeypleReaderException.h"
#include "../message/SeResponseSet.h"
#include "../message/SeResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;

                    KeypleReaderException::KeypleReaderException(const std::string &message) : KeypleBaseException(message) {
                    }

                    KeypleReaderException::KeypleReaderException(const std::string &message, std::runtime_error cause) : KeypleBaseException(message, cause) {
                    }

                    std::shared_ptr<SeResponseSet> KeypleReaderException::getSeResponseSet() {
                        return seResponseSet;
                    }

                    void KeypleReaderException::setSeResponseSet(std::shared_ptr<SeResponseSet> seResponseSet) {
                        this->seResponseSet = seResponseSet;
                    }

                    std::shared_ptr<SeResponse> KeypleReaderException::getSeResponse() {
                        return seResponse;
                    }

                    void KeypleReaderException::setSeResponse(std::shared_ptr<SeResponse> seResponse) {
                        this->seResponse = seResponse;
                    }
                }
            }
        }
    }
}
