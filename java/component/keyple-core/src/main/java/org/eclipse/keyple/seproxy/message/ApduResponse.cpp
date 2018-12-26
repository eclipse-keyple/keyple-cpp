#include "ApduResponse.h"
#include "../../util/ByteArrayUtils.h"
#include "Arrays.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    ApduResponse::ApduResponse(std::vector<char> &buffer, std::shared_ptr<std::set<int>> successfulStatusCodes) : bytes(buffer) {

                        if (buffer.empty()) {
                            this->successful = false;
                        }
                        else {
                            if (buffer.size() < 2) {
                                throw std::invalid_argument("Bad buffer (length < 2): " + std::to_string(buffer.size()));
                            }
                            int statusCode = ((buffer[buffer.size() - 2] & 0x000000FF) << 8) + (buffer[buffer.size() - 1] & 0x000000FF);
                            // java is signed only
                            if (statusCode < 0) {
                                statusCode += -2 * std::numeric_limits<short>::min();
                            }
                            if (successfulStatusCodes != nullptr) {
                                this->successful = statusCode == 0x9000 || (successfulStatusCodes->find(statusCode) != successfulStatusCodes->end());
                            }
                            else {
                                this->successful = statusCode == 0x9000;
                            }
                        }
                    }

                    bool ApduResponse::isSuccessful() {
                        return successful;
                    }

                    int ApduResponse::getStatusCode() {
                        int s = ((bytes[bytes.size() - 2] & 0x000000FF) << 8) + (bytes[bytes.size() - 1] & 0x000000FF);

                        // java is signed only
                        if (s < 0) {
                            s += -2 * std::numeric_limits<short>::min();
                        }
                        return s;
                    }

                    std::vector<char> ApduResponse::getBytes() {
                        return this->bytes;
                    }

                    std::vector<char> ApduResponse::getDataOut() {
                        return Arrays::copyOfRange(this->bytes, 0, this->bytes.size() - 2);
                    }

                    std::string ApduResponse::toString() {
                        std::string status = isSuccessful() ? "SUCCESS" : "FAILURE";
                        return "ApduResponse: " + status + ", RAWDATA = "
                                + ByteArrayUtils::toHex(this->bytes);
                    }

                    bool ApduResponse::equals(std::shared_ptr<void> o) {
                        if (o == shared_from_this()) {
                            return true;
                        }
                        if (!(std::static_pointer_cast<ApduResponse>(o) != nullptr)) {
                            return false;
                        }

                        std::shared_ptr<ApduResponse> resp = std::static_pointer_cast<ApduResponse>(o);
                        return Arrays::equals(resp->getBytes(), this->bytes) && resp->isSuccessful() == this->successful;
                    }

                    int ApduResponse::hashCode() {
                        int hash = 17;
                        hash = 19 * hash + (this->successful ? 0 : 1);
                        hash = 31 * hash + (bytes.empty() ? 0 : Arrays::hashCode(bytes));
                        return hash;
                    }
                }
            }
        }
    }
}
