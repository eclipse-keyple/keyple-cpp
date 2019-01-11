#pragma once

class System {
public:
	static long nanoTime()
	{
		timespec ts;
   		// clock_gettime(CLOCK_MONOTONIC, &ts); // Works on FreeBSD
   		clock_gettime(CLOCK_REALTIME, &ts);
		return ts.tv_sec *  pow(10, 9) + ts.tv_nsec;
	}

	static void arraycopy(std::vector<char> src, int srcPos, std::vector<char> dest, int destPos, int length)
	{
		for (int i = 0; i < length; i++) {
			dest[destPos + i] = src[srcPos + i];
		}
	}
};
