#include <iostream>

#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"
#include "CommandsTable.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace command {

using ApduRequest = org::eclipse::keyple::core::seproxy::message::ApduRequest;
using CommandsTable = org::eclipse::keyple::core::command::CommandsTable;

AbstractApduCommandBuilder::AbstractApduCommandBuilder(CommandsTable& commandReference, std::shared_ptr<ApduRequest> request)
: commandParserClass(commandReference.getCommandBuilderClass())
{
    std::cout << "here we go.........." << std::endl;

    this->name = commandReference.getName();
    this->request = request;
   
    // set APDU name for non null request
    if (request != nullptr) {
        this->request->setName(commandReference.getName());
    }
}

AbstractApduCommandBuilder::AbstractApduCommandBuilder(const std::string &name, std::shared_ptr<ApduRequest> request)
: commandParserClass(typeid(AbstractApduCommandBuilder))
{
    this->name = name;
    this->request = request;

    // set APDU name for non null request
    if (request != nullptr) {
        this->request->setName(name);
    }
}

void AbstractApduCommandBuilder::addSubName(const std::string &subName)
{
    if (subName.length() != 0) {
        this->name = this->name + " - " + subName;
        if (request != nullptr) {
            this->request->setName(this->name);
        }
    }
}

std::string AbstractApduCommandBuilder::getName()
{
    return this->name;
}

const std::type_info& AbstractApduCommandBuilder::getApduResponseParserClass()
{
    return this->commandParserClass;
}

std::shared_ptr<ApduRequest> AbstractApduCommandBuilder::getApduRequest()
{
    return request;
}

}
}
}
}
}
