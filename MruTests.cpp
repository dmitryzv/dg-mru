// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "MruCache.h"

bool InsertWorks()
{
    try
    {
        DZ_MRU::MruCache<int, std::string> mru(1);

        mru.insert(1, std::string("1"));
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool AtWorks()
{
    try
    {
        DZ_MRU::MruCache<int, std::string> mru(1);

        mru.insert(1, std::string("1"));

        if (*(mru.at(1)) != "1")
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool ObjectRemovedAfterReachingMaxSize()
{
    try
    {
        DZ_MRU::MruCache<int, std::string> mru(1);

        mru.insert(1, std::string("1"));
        mru.insert(2, std::string("2"));

        if (mru.at(1) != nullptr)
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool ValueReplacedForSameKey()
{
    try
    {
        DZ_MRU::MruCache<int, std::string> mru(1);

        mru.insert(1, std::string("1"));
        mru.insert(1, std::string("2"));

        if (*(mru.at(1)) != "2")
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool LastAccessedObjectRemovedOnInsert()
{
    try
    {
        DZ_MRU::MruCache<int, std::string> mru(2);

        mru.insert(1, std::string("1"));
        mru.insert(2, std::string("2"));

        if (*(mru.at(2)) != "2")
        {
            return false;
        }

        mru.insert(3, std::string("3"));

        // 1 should be removed
        if (mru.at(1) != nullptr)
        {
            return false;
        }

        // 2 should remain
        if (*(mru.at(2)) != "2")
        {
            return false;
        }

        // 3 should be inserted
        if (*(mru.at(3)) != "3")
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool Size0Works()
{
    try
    {
        DZ_MRU::MruCache<int, std::string> mru(0);

        mru.insert(1, std::string("1"));

        if (mru.at(1) != nullptr)
        {
            return false;
        }

        if (mru.at(2) != nullptr)
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

void main(int argc, char* argv[])
{
    bool rc(true);
    int testNum(0);

    if (rc && (rc = InsertWorks()))
    {
        ++testNum;
    }

    if (rc && (rc = AtWorks()))
    {
        ++testNum;
    }

    if (rc && (rc = ObjectRemovedAfterReachingMaxSize()))
    {
        ++testNum;
    }

    if (rc && (rc = ValueReplacedForSameKey()))
    {
        ++testNum;
    }

    if (rc && (rc = LastAccessedObjectRemovedOnInsert()))
    {
        ++testNum;
    }

    if (rc && (rc = Size0Works()))
    {
        ++testNum;
    }

    if (rc)
    {
        std::cout << "All tests pass" << std::endl;
    }
    else
    {
        std::cout << "Test: " << testNum << " failed." << std::endl;
    }
}

