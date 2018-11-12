/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file SeProxyService.cpp.
 *
 * \brief Implements the se proxy service class
 */

//#include "PlatformConfig.hpp"
#include "SeProxyService.hpp"
#include "ProxyReader.hpp"
#include "SeRequest.hpp"
#include <iostream>

using namespace keyple;
using namespace seproxy;
using namespace std;




    //
//    /*!
//	 * \fn uint32_t getVersion()
//	 *
//	 * \brief Gets the version level of the API
//	 *
//	 * \return The version.
//	 */
//
//    uint32_t getVersion()
//    {
//        DBG_TRACE_CALL();
//        return 1;
//    }
//
//    /**
//     * Sets the plugins.
//     *
//     * @param plugins the new plugins
//     */
//  public
//    void setPlugins(SortedSet<ReaderPlugin> plugins)
//    {
//        this.plugins = plugins;
//    }
//
//    /*!
//	 * \fn std::list<ReadersPlugin*> getPlugins()
//	 *
//	 * \brief Gets the readers plugins
//	 *
//	 * \return Null if it fails, else the plugins.
//	 */
//
//    std::list<ReadersPlugin *> getPlugins()
//    {
//        DBG_TRACE_CALL();
//        return this->plugins;
//    }
//
//    /*!
//	 * \fn SeProxyServiceImpl()
//	 *
//	 * \brief Default constructor
//	 */
//
//    SeProxyServiceImpl()
//    {
//        DBG_TRACE_CALL();
//    }
//
//    /*!
//	 * \fn ~SeProxyServiceImpl()
//	 *
//	 * \brief Destructor
//	 */
//
//    ~SeProxyServiceImpl()
//    {
//        DBG_TRACE_CALL();
//    }
//
//  private:
//    /*!
//	 * \fn SeProxyServiceImpl & operator= (const SeProxyServiceImpl*)
//	 *
//	 * \brief Assignment operator
//	 *
//	 * \param parameter1 The first parameter.
//	 *
//	 * \return A shallow copy of this object.
//	 */
//
//    SeProxyServiceImpl &operator=(const SeProxyServiceImpl *)
//    {
//    }
//
//    /*!
//	 * \fn SeProxyServiceImpl(const SeProxyServiceImpl*)
//	 *
//	 * \brief Constructor
//	 *
//	 * \param parameter1 The first parameter.
//	 */
//
//    SeProxyServiceImpl(const SeProxyServiceImpl *)
//    {
//    }
//
//    /*! \brief The instance */
//    static SeProxyServiceImpl m_instance;
//
//    /*! \brief The plugins */
//    std::list<ReadersPlugin *> plugins;
//};
//
///*! \brief The se proxy service implementation m instance */
//SeProxyServiceImpl SeProxyServiceImpl::m_instance = SeProxyServiceImpl();

/*!
 * \fn extern "C" EXPORT SeProxyService* CreateClass()
 *
 * \brief Creates the class
 *
 * \return Null if it fails, else the new class.
 */
#if defined(WIN32)
extern "C" __declspec(dllexport) SeProxyService &CreateClass()
{
    return SeProxyService::getInstance();
}
#endif
