#ifndef CONFIG_TREE_HPP
#define CONFIG_TREE_HPP
/******************************************************************************
*       Copyright Nokia 2019
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*       The code contains Nokia Confidential Proprietary Information.
*
******************************************************************************/
#include <cstring>
#include <exception>
#include <functional>
#include <iomanip>
#include <map>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <vector>

struct NameIdComparator
{
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        auto lhsPos = lhs.find('-');
        auto rhsPos = rhs.find('-');
        if (lhsPos == std::string::npos && rhsPos == std::string::npos) return lhs < rhs;
        std::string lhsName(lhs, 0, lhsPos);
        std::string rhsName(rhs, 0, rhsPos);
        if (lhsName != rhsName)
        {
            return lhsName < rhsName;
        }
        int lhsIndex = std::stol(&lhs[lhsPos + 1]);
        int rhsIndex = std::stol(&rhs[rhsPos + 1]);
        return lhsIndex < rhsIndex;
    }
};

struct ConfigTree;
using NameIdValueMap = std::map<std::string, ConfigTree, NameIdComparator>;

template <typename T>
void ConfigTreeValueHakaishin(void* pPtr)
{
    ((T*)(pPtr))->~T();
};

class ConfigTree
{
public:
    ConfigTree() = default;
    ~ConfigTree()
    {
        resetData();
    }

    ConfigTree(const ConfigTree&) = delete;

    ConfigTree(ConfigTree&& pOther)
    {
        resetData();
        mDataChoice = pOther.mDataChoice;
        mDataSz     = pOther.mDataSz;
        mDataType   = pOther.mDataType;
        if (DataChoice::PTR == mDataChoice)
        {
            mDataPtr = pOther.mDataPtr;
        }
        else if (DataChoice::RAW == mDataChoice)
        {
            std::memcpy(mDataRaw, pOther.mDataRaw, mDataSz);
        }
        pOther.mDataChoice     = DataChoice::UNSET;
        mDataDestructor        = pOther.mDataDestructor;
        pOther.mDataDestructor = nullptr;
        mChildren              = std::move(pOther.mChildren);
    }

    void resetData()
    {
        if (hasData() && mDataDestructor)
        {
            if (isPtr())
            {
                mDataDestructor(mDataPtr);
                delete[](uint8_t*) mDataPtr;
            }
            else
            {
                mDataDestructor(mDataRaw);
            }
        }
        mDataChoice     = DataChoice::UNSET;
        mDataDestructor = nullptr;
    }

    ConfigTree* at(const std::string& pPath)
    {
        auto i = get(pPath);
        return i.first == i.second ? nullptr : &i.first->second;
    }

    const ConfigTree* at(const std::string& pPath) const
    {
        auto i = get(pPath);
        return i.first == i.second ? nullptr : &i.first->second;
    }

    std::vector<std::pair<std::string, ConfigTree*>> all(const std::string pElem, const std::string pPath = "")
    {
        std::vector<std::pair<std::string, ConfigTree*>> rv;
        if (pElem != "*")
        {
            auto cmpElem = pElem + "-";
            auto i       = get(pPath + pElem + "-0");
            for (auto j = i.first; j != i.second; ++j)
            {
                if (j->first.size() >= cmpElem.size() && !std::memcmp(j->first.c_str(), cmpElem.c_str(), cmpElem.size()))
                {
                    rv.emplace_back(std::make_pair(j->first.c_str(), &j->second));
                    continue;
                }
                break;
            }
            return rv;
        }

        std::pair<NameIdValueMap::iterator, NameIdValueMap::iterator> i;
        if (pPath.size())
        {
            i        = get(pPath);
            i.first  = i.second->second.mChildren.begin();
            i.second = i.second->second.mChildren.end();
        }
        else
        {
            i.first  = mChildren.begin();
            i.second = mChildren.end();
        }
        for (auto j = i.first; j != i.second; ++j)
        {
            rv.emplace_back(std::make_pair(j->first.c_str(), &j->second));
        }
        return rv;
    }

    template <typename T>
    operator T&()
    {
        return getRefAs<T>();
    }

    template <typename T>
    bool isType() const
    {
        return (mDataType && mDataType->hash_code() == typeid(T).hash_code()) ? true : false;
    }

    template <typename T>
    T& getRefAs()
    {
        if (!hasData())
        {
            std::stringstream ss;
            ss << "Data not set for &ConfigTree=0x" << std::hex << uintptr_t(this) << std::endl;
            throw std::runtime_error(ss.str());
        }
        validateCast<T>();
        if (isPtr())
        {
            return *(T*)(mDataPtr);
        }
        return *(T*)(mDataRaw);
    }

    template <typename T>
    T* getPtrAs()
    {
        if (!hasData()) return nullptr;
        validateCast<T>();
        return isPtr() ? (T*)(mDataPtr) : (T*)(mDataRaw);
    }

    template <typename T>
    void setData(const T& pData)
    {
        resetData();
        mDataSz = sizeof(T);
        if (RAW_SIZE >= mDataSz)
        {
            new (mDataRaw) T(pData);
            mDataChoice = DataChoice::RAW;
        }
        else
        {
            mDataPtr = new uint8_t[sizeof(T)];
            new (mDataPtr) T(pData);
            mDataChoice = DataChoice::PTR;
        }
        mDataType       = &typeid(T);
        mDataDestructor = &ConfigTreeValueHakaishin<T>;
    }

    inline bool hasData() const
    {
        return DataChoice::UNSET != mDataChoice;
    }

    ConfigTree* createChild(const char* pName)
    {
        auto r = mChildren.emplace(std::make_pair(pName, ConfigTree()));
        return r.second ? &r.first->second : nullptr;
    }

private:
    template <typename T>
    void validateCast()
    {
        if (mDataType->hash_code() != typeid(T).hash_code())
        {
            std::stringstream ss;
            ss << "Data set for ConfigTree is " << mDataType->name() << " but " << typeid(T).name() << " is requested.";
            throw std::runtime_error(ss.str());
        }
    }

    // todo: only return an iterator
    std::pair<NameIdValueMap::iterator, NameIdValueMap::iterator> get(const std::string& pPath) const
    {
        /** TODO: improvement: this is non standard c++ **/
        char path[pPath.size() + 1];
        /** TODO: use std::string_view to avoid this kind of string processing**/
        std::strcpy(path, pPath.c_str());
        size_t      curPos      = 0;
        ConfigTree* currentNode = const_cast<ConfigTree*>(this);
        while (true)
        {
            size_t nextPos = pPath.find('/', curPos);
            if (nextPos != std::string::npos)
            {
                path[nextPos] = 0;
            }
            const char* cpath  = &path[curPos];
            auto        nodeIt = currentNode->mChildren.find(cpath);
            auto        endIt  = currentNode->mChildren.end();
            if (nodeIt == endIt)
            {
                return std::make_pair(currentNode->mChildren.end(), currentNode->mChildren.end());
            }
            currentNode = &nodeIt->second;
            if (nextPos == std::string::npos)
            {
                return std::make_pair(nodeIt, endIt);
            }
            curPos = nextPos + 1;
        }
    }

    inline bool isPtr()
    {
        return DataChoice::PTR == mDataChoice;
    }
    inline bool isRaw()
    {
        return DataChoice::RAW == mDataChoice;
    }
    static constexpr size_t RAW_SIZE = 64;
    union
    {
        void*   mDataPtr;
        uint8_t mDataRaw[RAW_SIZE];
    };
    enum class DataChoice : uint8_t
    {
        PTR,
        RAW,
        UNSET
    };

    DataChoice            mDataChoice = DataChoice::UNSET;
    size_t                mDataSz     = 0;
    const std::type_info* mDataType   = nullptr;
    NameIdValueMap        mChildren;
    void (*mDataDestructor)(void*) = nullptr;
};

#endif // CONFIG_TREE_HPP
