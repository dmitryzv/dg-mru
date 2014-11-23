#pragma once

#include <list>
#include <map>
#include <memory>
#include <Windows.h>

namespace DZCodeChallangeMRU
{
    // To simplify locking/unlocking and avoid unreleased locks in case of an exxception
    //  use helper class that aquires lock in constructor and releases in destructor
    //  kind of "Smart Lock"
    class AutoExlusiveSRWLock
    {
    public:
        AutoExlusiveSRWLock(PSRWLOCK lock)
            : m_lock(lock)
        {
            AcquireSRWLockExclusive(m_lock);
        }

        ~AutoExlusiveSRWLock()
        {
            ReleaseSRWLockExclusive(m_lock);
        }
    private:
        PSRWLOCK m_lock;
    };

    template <class K, class V> class MruCache
    {
    public:
        MruCache(size_t maxSize)
            : m_maxSize(maxSize)
        {
            InitializeSRWLock(&m_lock);
        }

        //  Function:
        //      insert
        //
        //  Purpose:  
        //      Inserts an element into MRU cache. If an element with same key
        //      exists old value is replaced by new value. If MRU cache exceeds max size
        //      oldest accessed element is removed.
        //  Parameters :
        //    const K& key  - key of the element
        //    V& value      - value of new element.  Aa copy of value is created upon insertion
        //  Returns:  void
        void insert(const K& key, V& value)
        {
            AutoExlusiveSRWLock lock(&m_lock);

            auto found = m_search.find(key);

            if (m_search.end() != found)
            {
                m_ordered.erase(found->second);
                m_ordered.emplace_front(std::make_pair(key, std::make_shared<V>(value)));
                found->second = m_ordered.begin();
            }
            else
            {
                if (m_maxSize == m_ordered.size())
                {
                    m_search.erase(m_ordered.back().first);
                    m_ordered.pop_back();
                }

                m_ordered.emplace_front(std::make_pair(key, std::make_shared<V>(value)));
                m_search.emplace(key, m_ordered.begin());
            }
        }

        //  Function:
        //      at
        //
        //  Purpose:  
        //      Retrieves an element from MRU cache. Accessing an element updates 
        //      its order for cache removal
        //  Parameters :
        //    const K& key  - key of the element
        //  Returns:  void
        //    std::shared_ptr<V> - value of the requested element. If an element with a requested key
        //                         is not in the cache nullptr is returned. Note that the returned type
        //                         is a shared_ptr and not a copy of the element. The 
        std::shared_ptr<V> at(const K& key)
        {
            AutoExlusiveSRWLock lock(&m_lock);

            auto found = m_search.find(key);

            if (m_search.end() != found)
            {
                m_ordered.splice(m_ordered.begin(), m_ordered, found->second);

                found->second = m_ordered.begin();

                return found->second->second;
            }

            return nullptr;
        }

    private:
        typedef std::pair<const K, std::shared_ptr<V>> ListEntryType;

        // Max number of elements to keep in the cache
        size_t m_maxSize;
        
        // A map between key and value to enable look up in O(logN)
        std::map<const K, typename std::list<ListEntryType>::iterator> m_search;
        
        // List of pointers to held elements. The list is kept in access sequence order
        //  from most to least recent
        std::list<ListEntryType> m_ordered;
        
        // Access lock 
        SRWLOCK m_lock;
    };
}