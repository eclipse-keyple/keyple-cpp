/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file pcscplugin/PcscPlugin.hpp.
 *
 * \brief Declares the PcscPlugin class
 */

#ifndef KEYPLE_SEPROXY_PCSC_READERS_PLUGIN_H
#define KEYPLE_SEPROXY_PCSC_READERS_PLUGIN_H

#include <string>
#include <list>
#include <thread>
#include <atomic>

#include "ProxyReader.hpp"
#include "ReaderPlugin.hpp"
#include "AbstractThreadedObservablePlugin.hpp"

using namespace keyple::seproxy;
using namespace keyple::seproxy::plugin;

namespace keyple {
    namespace plugin {
        namespace pcsc {

            /*!
             * \class PcscPlugin
             *
             * \brief PCSC plugin class.
             */
            class PcscPlugin : public AbstractThreadedObservablePlugin {
              public:
                /**
                 * Gets the single instance of PcscPlugin.
                 *
                 * @return single instance of PcscPlugin
                 */
                static PcscPlugin &getInstance()
                {
                    /**
                     * Singleton instance of SeProxyService
                     */
                    static PcscPlugin instance;

                    return instance;
                }

                /**
                 * @return the ‘unique’ name of the item
                 */
                const std::string getName()
                {
                    return "PcscPlugin";
                }

                /**
                 * Gets the parameters
                 *
                 * @return the configuration of the item
                 */
                std::map<std::string, std::string> *getParameters()
                {
                    return NULL;
                }

                /**
                 * allows to define a proprietary setting for a reader or a plugin (contactless protocols
                 * polling sequence, baud rate, … etc.).
                 *
                 * @param key the parameter key
                 * @param value the parameter value
                 * @throws IllegalArgumentException if the parameter or the value is not supported
                 * @throws KeypleBaseException if the parameter fails to be set up
                 */
                void setParameter(std::string key, std::string value)
                {
                }

                /**
                 * allows to define a set of proprietary settings for a reader or a plugin (contactless
                 * protocols polling sequence, baud rate, … etc.).
                 *
                 * @param parameters Parameters to setup
                 * @throws IllegalArgumentException if the parameters or the values is not supported
                 * @throws KeypleBaseException if the parameter fails to be set up
                 */
                void setParameters(std::map<std::string, std::string> parameters)
                {
                }

                /**
                 * Enable the logging
                 *
                 * @param logging If logging is enabled
                 * @return Same instance (fluent setter)
                 * @deprecated
                 */
                PcscPlugin *setLogging(bool logging);

                /**
                 * Gets the readers.
                 *
                 * @return the ‘unique’ name of the readers’ plugin.
                 * @throws KeypleReaderException if the list of readers has not been initialized
                 */
                std::set<ProxyReader> *getReaders()
                {
                    return NULL;
                }

                /**
                 * Gets the reader whose name is provided as an argument
                 * 
                 * @param name of the reader
                 * @return the ProxyReader object.
                 * @throws KeypleReaderNotFoundException if the wanted reader is not found
                 */
                ProxyReader *getReader(std::string name)
                {
                    return NULL;
                }

              private:
                bool logging = false;

                /*!
                 * \fn PcscPlugin::PcscPlugin();
                 *
                 * \brief Default constructor
                 */
                PcscPlugin();

                /*!
                 * \fn PcscPlugin::~PcscPlugin();
                 *
                 * \brief Destructor
                 */
                ~PcscPlugin();

                //  /*!
                //* \fn void PcscPlugin::destroy();
                //*
                //* \brief Destroys this object
                //*/

                //  void destroy();

                //  /*!
                //* \fn void PcscPlugin::readerPresenceMonitoringThread();
                //*
                //* \brief Reader presence monitoring thread
                //*/

                //  void readerPresenceMonitoringThread();

                //  PcscPlugin &operator=(const PcscPlugin *){};

                //  PcscPlugin(const PcscPlugin *){};

                //  static PcscPlugin m_instance;

                //  static std::atomic<bool> m_monitoring_is_running;

                //std::list<keyple::seproxy::ProxyReader *> m_readers_list;

                /* PCSC Specific */
                SCARDCONTEXT m_context;
            };
        } // namespace pcsc
    }     // namespace plugin
} // namespace keyple

#endif // KEYPLE_SEPROXY_PCSC_READERS_PLUGIN_H
