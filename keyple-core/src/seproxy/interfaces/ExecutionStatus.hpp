/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/*!
 * \file interfaces\ExecutionStatus.hpp.
 *
 * \brief Declares the ExecutionStatus enum
 */

#ifndef KEYPLE_SEPROXY_EXCECUTIONSTATUS_H
#define KEYPLE_SEPROXY_EXCECUTIONSTATUS_H

/**
 * \enum ExecutionStatus
 * \brief Error constants.
 *
 * ExecutionStatus defines the errors constants. 
 */
typedef enum ExecutionStatus
{
    // NO ERROR
    EXECUTION_OK = 0,

    // Exceptions/Errors raised by the reader
    CHANNEL_STATE_READER_ERROR = 1001, // the state of channel
    INVALID_APDU_READER_ERROR  = 1002, // unwaited format of APDU format for the reader
    IO_READER_ERROR            = 1003, // the I/O of the channel of the reader
    TIMEOUT_READER_ERROR       = 1004, // timeout of channel excedeed
    UNEXPECTED_READER_ERROR    = 1005,

    // Plugins errors
    INCONSISTENT_PARAMETER_VALUE_ERROR = 2001,
    PARAMETER_PROCESSING_ERROR_ERROR   = 2002,
    WRONG_PARAMETER_KEY_ERROR          = 2003,

    // Proxy Manager errors
    OUT_OF_MEMORY_ERROR                  = 3001,
    PLUGIN_NOT_SUPPORTED_ERROR           = 3002,
    FUNCTION_TABLE_NOT_INITIALIZED_ERROR = 3003,
    INVALID_PARAMETER_ERROR              = 3004,
    PLUGIN_INITIALIZATION_ERROR          = 3005,
    PLUGIN_SPECIFIC_ERROR                = 3006,
    INVALID_READER_INDEX_ERROR           = 3007,
    SE_NOT_FOUND                         = 3008,
    PLUGIN_ALREADY_SET_ERROR             = 3009

} ExecutionStatus;

#endif
