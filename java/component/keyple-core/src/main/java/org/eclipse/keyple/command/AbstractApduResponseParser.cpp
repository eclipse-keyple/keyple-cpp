#include "AbstractApduResponseParser.h"
#include "../seproxy/message/ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace command {
                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
const std::unordered_map<Integer, std::shared_ptr<StatusProperties>> AbstractApduResponseParser::STATUS_TABLE;

                AbstractApduResponseParser::StaticConstructor::StaticConstructor() {
                                            std::unordered_map<Integer, std::shared_ptr<StatusProperties>> m;
                                            m.emplace(0x9000, std::make_shared<StatusProperties>(true, "Success"));
                                            STATUS_TABLE = m;
                }

AbstractApduResponseParser::StaticConstructor AbstractApduResponseParser::staticConstructor;

                std::unordered_map<Integer, std::shared_ptr<StatusProperties>> AbstractApduResponseParser::getStatusTable() {
                    return STATUS_TABLE;
                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Deprecated public AbstractApduResponseParser(org.eclipse.keyple.seproxy.message.ApduResponse response)
                AbstractApduResponseParser::AbstractApduResponseParser(std::shared_ptr<ApduResponse> response) {
                    this->response = response;
                    initialized = true;
                }

                AbstractApduResponseParser::AbstractApduResponseParser() {
                    initialized = false;
                }

                void AbstractApduResponseParser::setApduResponse(std::shared_ptr<ApduResponse> response) {
                    this->response = response;
                    initialized = true;
                }

                bool AbstractApduResponseParser::isInitialized() {
                    return initialized;
                }

                std::shared_ptr<ApduResponse> AbstractApduResponseParser::getApduResponse() {
                    if (!initialized) {
                        throw std::make_shared<IllegalStateException>("The parser has not been initialized.");
                    }
                    return response;
                }

                int AbstractApduResponseParser::getStatusCode() {
                    return response->getStatusCode();
                }

                std::shared_ptr<StatusProperties> AbstractApduResponseParser::getPropertiesForStatusCode() {
                    return getStatusTable()[getStatusCode()];
                }

                bool AbstractApduResponseParser::isSuccessful() {
                    if (!initialized) {
                        throw std::make_shared<IllegalStateException>("The parser has not been initialized.");
                    }
                    std::shared_ptr<StatusProperties> props = getPropertiesForStatusCode();
                    return props != nullptr && props->isSuccessful();
                }

                std::string AbstractApduResponseParser::getStatusInformation() {
                    std::shared_ptr<StatusProperties> props = getPropertiesForStatusCode();
                    return props != nullptr ? props->getInformation() : "";
                }

                AbstractApduResponseParser::StatusProperties::StatusProperties(bool successful, const std::string &information) : successful(successful), information(information) {
                }

                bool AbstractApduResponseParser::StatusProperties::isSuccessful() {
                    return successful;
                }

                std::string AbstractApduResponseParser::StatusProperties::getInformation() {
                    return information;
                }
            }
        }
    }
}
