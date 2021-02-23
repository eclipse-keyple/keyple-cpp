/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <iterator>
#include <map>
#include <mutex>
#include <ostream>
#include <type_traits>

namespace keyple {
namespace common {

template <typename K, typename V>
class ConcurrentMap {
public:
    /**
     *
     */
    typedef K key_type;

    /**
     *
     */
    typedef std::pair<const K, V> value_type;

    /**
     *
     */
    typedef typename std::map<K, V>::iterator iterator;

    /**
     *
     */
    typedef typename std::map<K, V>::const_iterator const_iterator;

    /**
     *
     */
    typedef typename std::map<K, V>::size_type size_type;

    /**
     *
     */
    ConcurrentMap<K, V>() {}

    /**
     *
     */
    //ConcurrentMap<K, V>(const ConcurrentMap& other)
    //{
    //    this->mMap = other.mMap;
    //}

    /**
     *
     */
    ~ConcurrentMap<K, V>() = default;

    /**
     * Single element (1)
     */
    std::pair<iterator, bool> insert(const std::pair<const K, V>& val)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.insert(val);
    }

    /**
     * With hint (1)
     */
    iterator insert(const_iterator position, const value_type& val)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.insert(position, val);
    }

    /**
     * With hint (2)
     */
    template <class P>
    iterator insert(const_iterator position, P&& val)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.insert(position, val);
    }

    /**
     * Range
     */
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        mMap.insert(first, last);
    }

    /**
     *
     */
    size_type erase(const K& k)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        auto it = mMap.find(k);
        if (it != mMap.end())
            return mMap.erase(k);
        else
            return 0;
    }

    /**
     *
     */
    iterator erase(const_iterator position)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.erase(position);
    }

    /**
     *
     */
    size_type eraseFirstElement()
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        if (mMap.begin() == mMap.end())
            return 0;
        else
            return mMap.erase(mMap.begin()->first);
    }

    /**
     *
     */
    void clear() noexcept
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        mMap.clear();
    }

    /**
     *
     */
    const_iterator find(const key_type& k)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.find(k);
    }

    /**
     *
     */
    iterator begin() noexcept
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.begin();
    }

    /**
     *
     */
    const_iterator begin() const noexcept
    {
        return mMap.begin();
    }

    /**
     *
     */
    iterator end() noexcept
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.end();
    }

    /**
     *
     */
    const_iterator end() const noexcept
    {
        return mMap.end();
    }

    /**
     *
     */
    size_type count(const K& k) const
    {
        return mMap.count(k);
    }

    /**
     * Let's avoid using the mutex here as we would have to drop the 'const'
     * from the function (impacts in other classes I'd rather avoid)
     */
    size_type size() noexcept
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.size();
    }

    /**
     * Returns a reference to the mapped value of the element with key equivalent to key. If no such
     * element exists, an exception of type std::out_of_range is thrown.
     */
    V& at(const K& key)
    {
        return mMap.at(key);
    }

    const V& at(const K& key) const
    {
        return mMap.at(key);
    }

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const ConcurrentMap<K, V>& cm)
    {
        os << "CONCURRENTMAP: {";
        for (auto it = cm.begin(); it != cm.end(); ++it) {
            if (it != cm.end())
                os << ", ";
            os << "{" << it->first << ", " << it->second << "}";
        }
        os << "}";

        return os;
    }

private:
    /**
     *
     */
    std::map<K, V> mMap;

    /**
     *
     */
    std::mutex mMutex;
};

}
}
