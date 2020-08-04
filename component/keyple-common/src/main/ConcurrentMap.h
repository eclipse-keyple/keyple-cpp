/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#pragma once

#include <iterator>
#include <map>
#include <mutex>
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

private:

    /**
     *
     */
#ifndef _WIN32
    typedef typename __gnu_cxx
        ::__alloc_traits<std::allocator<std::pair<const K, V>>>::template
        rebind<value_type>::other _Pair_alloc_type;
#endif

    /**
     *
     */
#ifdef _WIN32
    typedef std::_Tree<std::_Tmap_traits<K, V, std::less<K>,
        std::allocator<std::pair<const K, V>>, false>> _Rep_type;
#else
    typedef std::_Rb_tree<key_type, value_type, std::_Select1st<value_type>,
        std::less<K>, _Pair_alloc_type> _Rep_type;
#endif

public:
    /**
     *
     */
    typedef typename _Rep_type::iterator iterator;

    /**
     *
     */
    typedef typename _Rep_type::const_iterator const_iterator;

    /**
     *
     */
    typedef typename _Rep_type::size_type size_type;

    /**
     *
     */
    ConcurrentMap<K, V>() {}

    /**
     *
     */
    ConcurrentMap<K, V>(const ConcurrentMap& other)
    {
        this->mMap = other.mMap;
    }

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
    iterator erase(const_iterator position)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        if (position != mMap.end())
            return mMap.erase(position);
        else
            return mMap.end();
    }

    /**
     *
     */
    size_type erase(const K& k)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.erase(k);
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
     */
    iterator begin() noexcept
    {
        return mMap.begin();
    }

    /**
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
    size_type count(const K& k)
    {
        const std::lock_guard<std::mutex> lock(mMutex);
        return mMap.count(k);
    }

    /**
     * Let's avoid using the mutex here as we would have to drop the 'const'
     * from the function (impacts in other classes I'd rather avoid)
     */
    size_type size() const noexcept
    {
        return mMap.size();
    }

    /**
     *
     */
    ConcurrentMap operator=(const ConcurrentMap& other)
    {
        this->mMap = other.mMap;

        return *this;
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
