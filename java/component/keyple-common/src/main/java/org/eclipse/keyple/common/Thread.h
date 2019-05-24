#pragma once

#include <chrono>
#include <thread>

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
    Thread() : name("Thread-x")
    {
        interrupted = false;
        detached = true;
        running = false;
    }

    /**
	 * Constructor
	 *
	 * Allocates a new Thread object. This constructor has the same effect as
	 * Thread (null, null, name).
	 *
	 * @param name the name of the new thread
	 */
    Thread(const std::string &name) : name(name)
    {
        interrupted = false;
        detached = true;
    }

    /**
	 * Destructor
	 */
    ~Thread()
    {
        if (running == 1 && detached == 0)
        {
            t->detach();
        }
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
        int result;

        interrupted = false;

        t = new std::thread(runThread, this);
        result = t ? 0 : -1;

        if (result == 0)
            running = 1;
    }

    /**
	 * In the call to pthread_create() the last argument is a void pointer to a
	 * data structure which will be passed to the runThread() function when it
	 * is called. Since the input argument to the runThread() is the Thread
	 * class this pointer, we can cast it to a Thread pointer then use it to
	 * call the Thread::run() method. Due to polymorphism, the Thread subclass
	 * run() method will be called to carry out the thread’s action.
	 */
    static void runThread(void *arg)
    {
        ((Thread *)arg)->run();
    }

    /**
	 * By default Pthreads are joinable. meaning you can wait for them to
	 * complete with a call to pthread_join(). The Thread class join method
	 * checks to see if the thread is running, then calls this function to wait
	 * for the thread to complete. If the call is successful the thread is
	 * marked as detached since pthread_join() automatically detatches a thread.
	 */
    int join()
    {
        int result = -1;

        if (running == 1) {
            t->join();
            detached = 1;
        }

        return result;
    }

    /**
	 * This is a utility method that detaches a thread when the caller doesn’t
	 * want to wait for the thread to complete. If the thread is running and not
	 * detached, pthread_detach() is called and the thread is flagged as
	 * detached if the call is successful.
	 */
    int detach()
    {
        int result = -1;

        if (running == 1 && detached == 0) {
            t->detach();
            detached = 1;
        }

        return result;
    }

    /**
	 * This is another utility method that returns the thread ID for display or
	 * logging purposes.
	 */
    std::thread::id self()
    {
        return t->get_id();
    }

    void setDaemon(bool on)
    {
        (void)on;
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
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
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

    virtual void *run() = 0;

    static std::thread::id currentThread()
    {
        return std::this_thread::get_id();
    }

private:
    /** */
    const std::string name;

    /** */
    bool interrupted;

    /** */
    std::thread *t;

    /** */
    int running;

    /** */
    int detached;
};
