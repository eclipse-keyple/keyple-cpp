/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include "AbstractLocalReader.hpp"
#include "ReaderEvent.hpp"
#include "ObservableReader.hpp"

namespace keyple {
    namespace seproxy {
        namespace plugin {

            /**
             *
             */
            class AbstractSelectionLocalReader : public virtual AbstractLocalReader,
                                                 public virtual ObservableReader {
              public:
                AbstractSelectionLocalReader(const std::string &pluginName, std::string &readerName)
                   : AbstractLocalReader(pluginName, readerName)
                {
                }

              private:
                /**
                 * Gets the SE Answer to reset
                 *
                 * @return ATR returned by the SE or reconstructed by the reader (contactless)
                 */
                virtual std::vector<uint8_t>* getATR()
                {
                    return nullptr;
                }

                /**
                 * Tells if the physical channel is open or not
                 *
                 * @return true is the channel is open
                 */
                virtual bool isPhysicalChannelOpen()
                {
                    return false;
                }
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple
