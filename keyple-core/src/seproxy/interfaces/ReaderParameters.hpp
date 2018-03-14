/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file ReaderParameters.hpp.
 *
 * \brief Declares the reader parameters
 */

#ifndef KEYPLE_SEPROXY_READER_PARAMETERS_H
#define KEYPLE_SEPROXY_READER_PARAMETERS_H

///< A flag that indicates whether other applications may form connections to the card.
#define PARAM_KEY_SHARE_MODE					"SHARE_MODE"
///< This application is willing to share the card with other applications.
#define PARAM_VALUE_SHARE_SHARED				"SHARED"
///< This application is not willing to share the card with other applications.
#define PARAM_VALUE_SHARE_EXCLUSIVE				"EXCLUSIVE"
///< This application is allocating the reader for its private use, and will be controlling it directly. No other applications are allowed access to it.
#define PARAM_VALUE_SHARE_DIRECT				"DIRECT"

///< A parameter that indicated the acceptable protocols for the connection.
#define PARAM_KEY_PROTOCOL						"PROTOCOL"
///< T=0 is an acceptable protocol.
#define PARAM_VALUE_PROTOCOL_T0					"T0"
///< T=1 is an acceptable protocol.
#define PARAM_VALUE_PROTOCOL_T1					"T1"
///< Both T=0 and T=1 are an acceptable protocol.
#define PARAM_VALUE_PROTOCOL_T0T1				"T*"


///< Action to take on the card in the connected reader on close.
#define PARAM_KEY_DISPOSITION					"DISPOSITION"
///< Do not do anything special.
#define PARAM_VALUE_DISPOSITION_LEAVE			"LEAVE"
///< Reset the card.
#define PARAM_VALUE_DISPOSITION_RESET			"RESET"
///< Power down the card.
#define PARAM_VALUE_DISPOSITION_UNPOWER			"UNPOWER"
///< Eject the card.
#define PARAM_VALUE_DISPOSITION_EJECT			"EJECT"

#endif // KEYPLE_SEPROXY_READER_PARAMETERS_H
