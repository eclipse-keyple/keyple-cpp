/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <math.h>
#include <vector>
#include <memory>

#if defined(_WIN32)
	#include <windows.h>
#endif

namespace keyple {
namespace common {

class System {
public:
	static long nanoTime()
	{
#if defined(WIN32)
		SYSTEMTIME time;
		GetSystemTime(&time);
		return static_cast<long>(((time.wSecond * 1000) + time.wMilliseconds) * pow(10, 6));
#else
		timespec ts;
   		// clock_gettime(CLOCK_MONOTONIC, &ts); // Works on FreeBSD
   		clock_gettime(CLOCK_REALTIME, &ts);
		return ts.tv_sec *  pow(10, 9) + ts.tv_nsec;
#endif
	}

	static void arraycopy(const std::vector<char>& src, int srcPos, std::vector<char>& dest, int destPos, int length)
	{
		for (int i = 0; i < length; i++) {
			dest[destPos + i] = src[srcPos + i];
		}
	}

    static long currentTimeMillis()
	{
        return (long)(nanoTime() * pow(10, 6));
    }
};

}
}
