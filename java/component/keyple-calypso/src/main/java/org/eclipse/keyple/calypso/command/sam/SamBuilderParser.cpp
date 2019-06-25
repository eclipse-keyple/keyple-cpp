#include "SamBuilderParser.h"
#include "AbstractSamCommandBuilder.h"
#include "AbstractSamResponseParser.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        using CalypsoBuilderParser = org::eclipse::keyple::calypso::command::CalypsoBuilderParser;

                        SamBuilderParser::SamBuilderParser(std::shared_ptr<AbstractSamCommandBuilder> samCommandBuilder) : samCommandBuilder(samCommandBuilder) {
                        }

                        std::shared_ptr<AbstractSamCommandBuilder> SamBuilderParser::getCommandBuilder() {
                            return samCommandBuilder;
                        }

                        std::shared_ptr<AbstractSamResponseParser> SamBuilderParser::getResponseParser() {
                            return samResponseParser;
                        }

                        void SamBuilderParser::setResponseParser(std::shared_ptr<AbstractSamResponseParser> poResponseParser) {
                            this->samResponseParser = poResponseParser;
                        }
                    }
                }
            }
        }
    }
}
