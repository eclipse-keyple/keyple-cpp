/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include <iostream>
#include <winscard.h>

//#include "PlatformConfig.hpp"
#include "ReaderPlugin.hpp"
#include "ProxyReader.hpp"
#include "PcscPlugin.hpp"
#include "PcscReader.hpp"

using namespace keyple::plugin::pcsc;

PcscPlugin::PcscPlugin() : AbstractThreadedObservablePlugin("PcscPlugin")
{
    //DBG_TRACE_CALL();
    LONG lReturn;
    this->m_context = 0;
    //m_monitoring_is_running = false;

    /// Establish the PC/SC context.
    lReturn = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &this->m_context);
    if (SCARD_S_SUCCESS != lReturn)
    {
        //DBG_ERROR_MSG("Failed SCardEstablishContext");
        // TODO define behaviour
    }
    else
    {
        // Use the context as needed. When done,
        // free the context by calling SCardReleaseContext.
        // ...
    }
}

PcscPlugin::~PcscPlugin()
{
    //DBG_TRACE_CALL();
}

PcscPlugin *PcscPlugin::setLogging(boolean logging)
{
    this->logging = logging;
    return this;
}

//void PcscPlugin::destroy()
//{
//    /// Stop the monitoring thread.
//    /// TODO m_monitoring_is_running = false;
//
//    /// Delete all reader objects.
//    std::list<ProxyReader *>::iterator iterator_reader = m_readers_list.begin();
//    for (uint32_t ii = 0; ii < m_readers_list.size(); ii++)
//    {
//        ProxyReader *proxy_reader = *iterator_reader;
//        delete proxy_reader;
//        iterator_reader++;
//    }
//
//    /// Release the PC/SC context.
//    if (this->m_context != 0)
//    {
//        SCardReleaseContext(this->m_context);
//    }
//
//    delete this;
//}

///*!
// * \fn void PcscPlugin::readerPresenceMonitoringThread()
// *
// * \brief Reader presence monitoring thread (experimental)
// */
//
//void PcscPlugin::readerPresenceMonitoringThread()
//{
//    uint32_t lReturn;
//    SCARD_READERSTATE rgscState;
//    const char *readerNotification = "\\\\?PnP?\\Notification";
//
//    memset(&rgscState, 0, sizeof(rgscState));
//
//    rgscState.szReader       = readerNotification;
//    rgscState.dwCurrentState = SCARD_STATE_UNAWARE;
//
//    while (true)
//    {
//        lReturn                  = SCardGetStatusChange(this->m_context,
//                                       INFINITE, // infinite wait
//                                       &rgscState, 1);
//        rgscState.dwCurrentState = rgscState.dwEventState;
//        if (lReturn == SCARD_S_SUCCESS)
//        {
//            DBG_ERROR_MSG("SCardGetStatusChange returned " << std::hex << lReturn);
//        }
//        else
//        {
//            DBG_ERROR_MSG("SCardGetStatusChange returned " << std::hex << lReturn);
//        }
//    }
//}
//
///*! \brief The pcsc plugin m instance */
//PcscPlugin PcscPlugin::m_instance = PcscPlugin();
//
/*!
 * \fn extern "C" EXPORT ReadersPlugin* CreateClass()
 *
 * \brief Creates the class ("singleton in library" pattern)
 *
 * \return Null if it fails, else the new class.
 */
extern "C" __declspec(dllexport) PcscPlugin &CreateClass()
{
    //DBG_TRACE_CALL();
    return PcscPlugin::getInstance();
}
