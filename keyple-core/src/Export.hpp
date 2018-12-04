/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License version 2.0 which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#ifndef KEYPLE_SEPROXY_EXPORT_H
#define KEYPLE_SEPROXY_EXPORT_H

#if defined(WIN32)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#endif /* KEYPLE_SEPROXY_EXPORT_H */