/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include <chrono>
#include <condition_variable>
#include <mutex>

class CountDownLatch 
{
public:
    explicit CountDownLatch(const unsigned int count): m_count(count) { }
    virtual ~CountDownLatch() = default;

    void await(void) 
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_count > 0) {
            m_cv.wait(lock, [this](){ return m_count == 0; });
        }
    }

    template <class Rep, class Period>
    bool await(const std::chrono::duration<Rep, Period>& timeout)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        bool result = true;
        if (m_count > 0) 
        {
            result = m_cv.wait_for(lock, timeout, [this](){ return m_count == 0; });
        }

        return result;
    }

    void countDown(void) 
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_count > 0) 
        {
            m_count--;
            m_cv.notify_all();
        }
    }

    unsigned int getCount(void) 
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_count;
    }

protected:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    unsigned int m_count = 0;
};
