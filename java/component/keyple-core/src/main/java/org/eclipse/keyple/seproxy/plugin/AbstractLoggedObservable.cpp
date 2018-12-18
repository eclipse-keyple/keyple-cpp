#include "AbstractLoggedObservable.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                    using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using Configurable = org::eclipse::keyple::util::Configurable;
                    using Nameable = org::eclipse::keyple::util::Nameable;
                    using Observable = org::eclipse::keyple::util::Observable;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
                }
            }
        }
    }
}
