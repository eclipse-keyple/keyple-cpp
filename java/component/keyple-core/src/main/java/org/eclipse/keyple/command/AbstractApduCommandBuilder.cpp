#include "AbstractApduCommandBuilder.h"
#include "../seproxy/message/ApduRequest.h"
#include "CommandsTable.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace command {
                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;

                AbstractApduCommandBuilder::AbstractApduCommandBuilder(std::shared_ptr<CommandsTable> commandReference, std::shared_ptr<ApduRequest> request) {
                    this->name = commandReference->getName();
                    this->commandParserClass = commandReference->getResponseParserClass();
                    this->request = request;
                    // set APDU name for non null request
                    if (request != nullptr) {
                        this->request->setName(commandReference->getName());
                    }
                }

                AbstractApduCommandBuilder::AbstractApduCommandBuilder(const std::string &name, std::shared_ptr<ApduRequest> request) {
                    this->name = name;
                    this->request = request;
                    this->commandParserClass = nullptr;
                    // set APDU name for non null request
                    if (request != nullptr) {
                        this->request->setName(name);
                    }
                }

                void AbstractApduCommandBuilder::addSubName(const std::string &subName) {
                    if (subName.length() != 0) {
                        this->name = this->name + " - " + subName;
                        if (request != nullptr) {
                            this->request->setName(this->name);
                        }
                    }
                }

                std::string AbstractApduCommandBuilder::getName() {
                    return this->name;
                }

                std::type_info AbstractApduCommandBuilder::getApduResponseParserClass() {
                    return this->commandParserClass;
                }

                std::shared_ptr<ApduRequest> AbstractApduCommandBuilder::getApduRequest() {
                    return request;
                }
            }
        }
    }
}
