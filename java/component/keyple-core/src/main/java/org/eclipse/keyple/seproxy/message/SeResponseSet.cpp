#include "SeResponseSet.h"
#include "SeResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {

                    std::vector<std::shared_ptr<SeResponse>> SeResponseSet::getResponses() {
                        return seResponses;
                    }

                    SeResponseSet::SeResponseSet(std::vector<std::shared_ptr<SeResponse>> &seResponses) : seResponses(seResponses) {
                    }

                    SeResponseSet::SeResponseSet(std::shared_ptr<SeResponse> response) : seResponses(seResponses) {
                        std::vector<std::shared_ptr<SeResponse>> seResponses;
                        seResponses.push_back(response);
                    }

                    std::shared_ptr<SeResponse> SeResponseSet::getSingleResponse() {
                        if (seResponses.size() != 1) {
                            throw std::make_shared<IllegalStateException>("This method only support ONE element");
                        }
                        return seResponses[0];
                    }

                    std::string SeResponseSet::toString() {
                        return std::to_string(seResponses.size()) + " SeReponse(s)";
                    }
                }
            }
        }
    }
}
