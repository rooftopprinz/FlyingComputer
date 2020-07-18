#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP
/******************************************************************************
*       Copyright Nokia 2019
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*       The code contains Nokia Confidential Proprietary Information.
*
******************************************************************************/
#include "ConfigProcessor.hpp"

struct CfgLoader
{
    CfgLoader(const char* name, uint8_t** cursor, int64_t* size, ConfigTree* cfgTree)
        : name(name)
        , cursor(cursor)
        , size(size)
    {
        // LM(LD, "PK Loading %s", name)
        if (!cfgTree)
        {
            throw std::runtime_error("ConfigTree can't be nullptr for CfgLoader!");
        }

        if (name[0] == '.')
        {
            this->cfgTree = cfgTree;
        }
        else if (!(this->cfgTree = cfgTree->at(name)))
        {
            // LM(LE, "Can't find child: %s", name);
            this->cfgTree = cfgTree;
        }
    }

    ConfigTree* checkNode(const char* name)
    {
        // LM(LD, "PK checking Node: %s", name)
        auto node = cfgTree->at(name);
        if (!node)
        {
            // LM(LE, "ConfigLoader Error! Can't find requested children: \"%s", name, "\"");
            return nullptr;
        }
        return node;
    }

    ~CfgLoader()
    {
    }

    template <typename T>
    typename std::enable_if<!IsCfgConstructible<T>::value && std::is_integral<T>::value>::type loadCfg(const char* name, std::vector<T>* va)
    {
        auto ct = cfgTree->all(name);
        va->clear();
        for (auto i : ct)
        {
            if (!i.second->isType<unsigned long long>())
            {
                throw std::runtime_error(std::string("invalid type in field: ")+name);
            }
            va->push_back((T)i.second->getRefAs<unsigned long long>());
        }
    }

    template <typename T>
    typename std::enable_if<!IsCfgConstructible<T>::value && std::is_floating_point<T>::value>::type loadCfg(const char* name, std::vector<T>* va)
    {
        auto ct = cfgTree->all(name);
        va->clear();
        for (auto i : ct)
        {
            if (!i.second->isType<long double>() && !i.second->isType<unsigned long long>())
            {
                throw std::runtime_error(std::string("invalid type in field: ")+name);
            }
            T val = i.second->isType<long double>() ? static_cast<T>(i.second->getRefAs<long double>()) : i.second->getRefAs<unsigned long long>();
            va->push_back(val);
        }
    }

    template <typename T>
    typename std::enable_if<!IsCfgConstructible<T>::value && !std::is_integral<T>::value && !std::is_floating_point<T>::value>::type
    loadCfg(const char* name, std::vector<T>* va)
    {
        auto ct = cfgTree->all(name);
        va->clear();
        for (auto i : ct)
        {
            if (i.second->isType<T>())
            {
                throw std::runtime_error(std::string("invalid type in field: ")+name);
            }
            va->push_back(i.second->getRefAs<T>());
        }
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type loadCfg(const char* name, std::vector<T>* va)
    {
        auto ct = cfgTree->all(name);
        va->clear();
        for (auto i : ct)
        {
            va->emplace_back();
            CfgProcessor<T> cp;
            cp.cfgTree = cfgTree;
            cp.template process<CfgLoader>(i.first.c_str(), &va->back());
        }
    }

    template <typename T>
    typename std::enable_if<!IsCfgConstructible<T>::value && std::is_integral<T>::value>::type loadCfg(const char* name, T* va)
    {
        auto node     = checkNode(name);
        if (node)
        {
            if (!node->isType<unsigned long long>())
            {
                throw std::runtime_error(std::string("invalid type in field: ")+name);
            }
            *va = (unsigned long long)(*node);
        }

    }

    template <typename T>
    typename std::enable_if<!IsCfgConstructible<T>::value && std::is_floating_point<T>::value>::type loadCfg(const char* name, T* va)
    {
        auto node     = checkNode(name);
        if (node)
        {
            if (!node->isType<long double>() && !node->isType<unsigned long long>())
            {
                throw std::runtime_error(std::string("invalid type in field: ")+name);
            }
            *va = node->isType<long double>() ? (T)(long double)(*node) : (T)(long)(unsigned long long)(*node);
        }
    }

    template <typename T>
    typename std::enable_if<!IsCfgConstructible<T>::value && !std::is_integral<T>::value && !std::is_floating_point<T>::value>::type
    loadCfg(const char* name, T* va)
    {
        auto node     = checkNode(name);
        if (node)
        {
            if (!node->isType<T>())
            {
                throw std::runtime_error(std::string("invalid type in field: ")+name);
            }
            *va = node->getRefAs<T>();
        }
    }

    void loadCfg(const char* name, std::string* va)
    {
        auto node     = checkNode(name);
        if (node)
        {
            if (!node->isType<std::string>())
            {
                throw std::runtime_error(std::string("invalid type in field: ")+name);
            }
            *va = node->getRefAs<std::string>();
        }
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type loadCfg(const char* name, T* va)
    {
        CfgProcessor<T> cp;
        cp.cfgTree = cfgTree;
        cp.template process<CfgLoader>(name, va);
    }

    template <typename T>
    void operator()(const char* n, T* v)
    {
        loadCfg(n, v);
    }

    template <typename T>
    CfgLoader& operator<<(std::pair<const char*, T*>&& v)
    {
        loadCfg(v.first, v.second);
        return *this;
    }

    const char* name;
    uint8_t**   cursor;
    int64_t*    size;
    ConfigTree* cfgTree;
};

template <typename T>
void LoadConfig(const char* name, ConfigTree* cfgTree, T& output)
{
    CfgProcessor<T> cp;
    cp.cfgTree = cfgTree;
    cp.template process<CfgLoader>(name, &output);
}

#endif // CONFIG_LOADER_HPP
