#pragma once

class System {
public:
	static long nanoTime() {
		timespec ts;
   		// clock_gettime(CLOCK_MONOTONIC, &ts); // Works on FreeBSD
   		clock_gettime(CLOCK_REALTIME, &ts);
		return ts.tv_sec *  pow(10, 9) + ts.tv_nsec;
	}
};
