#include "AbstractIso7816CommandBuilder.h"
#include "CommandsTable.h"
#include "ApduRequest.h"

/* Common */
#include "System.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace command {
                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;

                AbstractIso7816CommandBuilder::AbstractIso7816CommandBuilder(std::shared_ptr<CommandsTable> commandReference, std::shared_ptr<ApduRequest> request) : AbstractApduCommandBuilder(commandReference, request) {
                }

                AbstractIso7816CommandBuilder::AbstractIso7816CommandBuilder(const std::string &name, std::shared_ptr<ApduRequest> request) : AbstractApduCommandBuilder(name, request) {
                }

                std::shared_ptr<ApduRequest> AbstractIso7816CommandBuilder::setApduRequest(char cla, std::shared_ptr<CommandsTable> command, char p1, char p2, std::vector<char> &dataIn, char le) {
                    bool case4;
                    /* sanity check */
                    if (dataIn.size() > 0 && le != -1 && le != 0) {
                        throw std::invalid_argument("Le must be equal to 0 when not null and ingoing data are present.");
                    }

                    /* Buffer allocation */
                    int length = 4; // header
                    if (dataIn.size() > 0) {
                        length += dataIn.size() + 1; // Lc + data
                    }
                    if (le != -1) {
                    //if (le != nullptr) {
                        length += 1; // Le
                    }
                    
                    std::vector<char> apdu(length);

                    /* Build APDU buffer from provided arguments */
                    apdu[0] = cla;
                    apdu[1] = command->getInstructionByte();
                    apdu[2] = p1;
                    apdu[3] = p2;

                    /* ISO7618 case determination and Le management */
                    if (dataIn.size() > 0) {
                        /* append Lc and ingoing data */
                        apdu[4] = static_cast<char>(dataIn.size());
                        System::arraycopy(dataIn, 0, apdu, 5, dataIn.size());
                        if (le != -1) {
                            /*
                             * case4: ingoing and outgoing data, Le is always set to 0 (see Calypso Reader
                             * Recommendations - T84)
                             */
                            case4 = true;
                            apdu[length - 1] = le;
                        }
                        else {
                            /* case3: ingoing data only, no Le */
                            case4 = false;
                        }
                    }
                    else {
                        if (le != -1) {
                            /* case2: outgoing data only */
                            apdu[4] = le;
                        }
                        else {
                            /* case1: no ingoing, no outgoing data, P3/Le = 0 */
                            apdu[4] = static_cast<char>(0x00);
                        }
                        case4 = false;
                    }

                    return std::make_shared<ApduRequest>(command->getName(), apdu, case4);
                }
            }
        }
    }
}
