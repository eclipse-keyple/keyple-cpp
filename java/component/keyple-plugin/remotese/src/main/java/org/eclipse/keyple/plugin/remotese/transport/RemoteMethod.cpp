#include "RemoteMethod.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {

RemoteMethod RemoteMethod::READER_TRANSMIT("READER_TRANSMIT", InnerEnum::READER_TRANSMIT, "reader_transmit");
RemoteMethod RemoteMethod::READER_CONNECT("READER_CONNECT", InnerEnum::READER_CONNECT, "reader_connect");
RemoteMethod RemoteMethod::READER_DISCONNECT("READER_DISCONNECT", InnerEnum::READER_DISCONNECT, "reader_disconnect");
RemoteMethod RemoteMethod::READER_EVENT("READER_EVENT", InnerEnum::READER_EVENT, "reader_event");
RemoteMethod RemoteMethod::DEFAULT_SELECTION_REQUEST("DEFAULT_SELECTION_REQUEST", InnerEnum::DEFAULT_SELECTION_REQUEST, "default_selection_request");

std::vector<RemoteMethod> RemoteMethod::valueList;
int RemoteMethod::nextOrdinal = 0;

                        RemoteMethod::RemoteMethod(const std::string &name, InnerEnum innerEnum, const std::string &name) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                            this->name = name;
                        }

                        std::string RemoteMethod::getName() {
                            return this->name;
                        }

const std::unordered_map<std::string, RemoteMethod> RemoteMethod::lookup = std::unordered_map<std::string, RemoteMethod>();
RemoteMethod::StaticConstructor RemoteMethod::staticConstructor;

                        RemoteMethod RemoteMethod::get(const std::string &name) {
                            return lookup->get(name);
                        }

bool RemoteMethod::operator == (const RemoteMethod &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool RemoteMethod::operator != (const RemoteMethod &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<RemoteMethod> RemoteMethod::values() {
    return valueList;
}

int RemoteMethod::ordinal() {
    return ordinalValue;
}

std::string RemoteMethod::toString() {
    return nameValue;
}

RemoteMethod RemoteMethod::valueOf(const std::string &name) {
    for (auto enumInstance : RemoteMethod::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }
}
                    }
                }
            }
        }
    }
}
