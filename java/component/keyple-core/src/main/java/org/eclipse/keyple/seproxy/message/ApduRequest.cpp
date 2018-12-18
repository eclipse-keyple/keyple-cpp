#include <sstream>

#include "ApduRequest.h"
#include "../../util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    ApduRequest::ApduRequest(std::vector<char> &buffer, bool case4, std::shared_ptr<std::set<int>> successfulStatusCodes) : case4(case4), successfulStatusCodes(successfulStatusCodes) {
                        this->bytes = buffer;
                    }

                    ApduRequest::ApduRequest(const std::string &name, std::vector<char> &buffer, bool case4, std::shared_ptr<std::set<int>> successfulStatusCodes) : ApduRequest(buffer, case4, successfulStatusCodes) {
                        this->name = name;
                    }

                    ApduRequest::ApduRequest(std::vector<char> &buffer, bool case4) : ApduRequest(buffer, case4, nullptr) {
                    }

                    ApduRequest::ApduRequest(const std::string &name, std::vector<char> &buffer, bool case4) : ApduRequest(buffer, case4, nullptr) {
                        this->name = name;
                    }

                    bool ApduRequest::isCase4() {
                        return case4;
                    }

                    void ApduRequest::setName(const std::string &name) {
                        this->name = name;
                    }

                    std::shared_ptr<std::set<int>> ApduRequest::getSuccessfulStatusCodes() {
                        return successfulStatusCodes;
                    }

                    std::string ApduRequest::getName() {
                        return name;
                    }

                    std::vector<char> ApduRequest::getBytes() {
                        return this->bytes;
                    }

                    std::string ApduRequest::toString() {
                        std::stringstream string;
                        string << "ApduRequest: NAME = " << this->getName() << ", RAWDATA = " << ByteArrayUtils::toHex(bytes);
                        if (isCase4()) {
                            string << ", case4";
                        }
                        if (successfulStatusCodes != nullptr) {
                            string << ", additional successful status codes = ";
                            std::set<int>::const_iterator iterator = successfulStatusCodes->begin();
                            while (iterator != successfulStatusCodes->end()) {
                                string << std::hex << *iterator;
                                if (iterator != successfulStatusCodes->end()) {
                                    string << ", ";
                                }
                                iterator++;
                            }
                        }
                        return string.str();
                    }
                }
            }
        }
    }
}
