#pragma once

#if defined(_WIN32)
	#include <windows.h>
#else
	#include <unistd.h>
#endif

#include "Object.h"

class Thread {
public:
	/**
	 * Constructor
	 *
	 * Allocates a new Thread object. This constructor has the same effect as
	 * Thread (null, null, gname), where gname is a newly generated name.
	 * Automatically generated names are of the form "Thread-"+n, where n is an
	 * integer.
	 */
	Thread() : name("Thread-x"), interrupted(false)
	{
	}

	/**
	 * Constructor
	 *
	 * Allocates a new Thread object. This constructor has the same effect as
	 * Thread (null, null, name).
	 *
	 * @param name the name of the new thread
	 */
	Thread(const std::string &name) : name(name), interrupted(true)
	{
	}

	/**
	 * Causes this thread to begin execution.
	 *
	 * The result is that two threads are running concurrently: the current
	 * thread (which returns from the call to the start method) and the other
	 * thread (which executes its run method).
	 *
	 * It is never legal to start a thread more than once. In particular, a
	 * thread may not be restarted once it has completed execution.
	 *
	 * @throws IllegalThreadStateException if the thread was already started.
	 */
	void start()
	{
		interrupted = false;
	}

	void setDaemon(bool on)
	{
	}

	/**
	 * Causes the currently executing thread to sleep (temporarily cease
	 * execution) for the specified number of milliseconds, subject to the
	 * precision and accuracy of system timers and schedulers. The thread does
	 * not lose ownership of any monitors.
	 *
	 * @param millis the length of time to sleep in milliseconds
	 *
	 * @throws IllegalArgumentException if the value of millis is negative
	 * @throws InterruptedException if any thread has interrupted the current
	 *                              thread. The interrupted status of the
	 *                              current thread is cleared when this
	 *                              exception is thrown.static void sleep(long
	 *                              millis) throw(InterruptedException)
	 */
	static void sleep(long millis)
	{
#if defined(_WIN32)
		Sleep(millis);
#else
		usleep(millis * 1000);
#endif
	}

	/**
	 * Interrupts this thread.
	 *
	 * Unless the current thread is interrupting itself, which is always
	 * permitted, the checkAccess method of this thread is invoked, which may
	 * cause a SecurityException to be thrown.
	 *
	 * If this thread is blocked in an invocation of the wait(), wait(long), or
	 * wait(long, int) methods of the Object class, or of the join(),
	 * join(long), join(long, int), sleep(long), or sleep(long, int), methods of
	 * this class, then its interrupt status will be cleared and it will receive
	 * an InterruptedException.
	 *
	 * If this thread is blocked in an I/O operation upon an interruptible
	 * channel then the channel will be closed, the thread's interrupt status
	 * will be set, and the thread will receive a ClosedByInterruptException.
	 *
	 * If this thread is blocked in a Selector then the thread's interrupt
	 * status will be set and it will return immediately from the selection
	 * operation, possibly with a non-zero value, just as if the selector's
	 * wakeup method were invoked.
	 *
	 * If none of the previous conditions hold then this thread's interrupt
	 * status will be set.
	 *
	 * Interrupting a thread that is not alive need not have any effect.
	 *
	 * @throws SecurityException if the current thread cannot modify this thread
	 */
	void interrupt()
	{
		interrupted = true;
	}

private:
	/**
	 *
	 */
	const std::string name;

	/**
	 *
	 */
	bool interrupted;
};
