/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_SEPROXY_SERVICE
#define KEYPLE_SEPROXY_SEPROXY_SERVICE

namespace keyple {
    namespace seproxy {

        /**
         * The Class SeProxyService. This singleton is the entry point of the SE Proxy Service, its instance
         * has to be called by a ticketing application in order to establish a link with a SE’s application.
         *
         */
        class SeProxyService {
          public:
            /**
             * Singleton instance of SeProxyService
             */
            static SeProxyService &getInstance()
            {
                static SeProxyService instance;
                return instance;
            }

          private:
            /**
             * Gets the single instance of SeProxyService.
             *
             * @return single instance of SeProxyService
             */
            SeProxyService() {}
        };
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLE_SEPROXY_SEPROXY_SERVICE
