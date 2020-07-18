#ifndef CONFIG_SIZER_HPP
#define CONFIG_SIZER_HPP
/******************************************************************************
*       Copyright Nokia 2019
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*       The code contains Nokia Confidential Proprietary Information.
*
******************************************************************************/
#include "ConfigProcessor.hpp"

struct CfgSizer
{
    CfgSizer(const char* name, uint8_t** cursor, int64_t* size, ConfigTree* cfgTree)
        : name(name)
        , cursor(cursor)
        , size(size)
    {
    }

    ~CfgSizer()
    {
    }

    template <typename T>
    typename EnableIfNotCfgConstructible<T>::type sizeCfg(const char* name, std::vector<T>* va)
    {
        *size += sizeof(uint32_t);
        *size += va->size() * sizeof(T);
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type sizeCfg(const char* name, std::vector<T>* va)
    {
        *size += sizeof(uint32_t);
        for (T& i : *va)
        {
            CfgProcessor<T> cp;
            cp.size = size;
            cp.template process<CfgSizer>(name, &i);
        }
    }

    template <typename T>
    typename EnableIfNotCfgConstructible<T>::type sizeCfg(const char* name, T* va)
    {
        *size += sizeof(T);
    }

    void sizeCfg(const char* name, std::string* va)
    {
        *size += va->size() + 1;
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type sizeCfg(const char* name, T* va)
    {
        CfgProcessor<T> cp;
        cp.cursor = cursor;
        cp.size   = size;
        cp.template process<CfgSizer>(name, va);
    }

    template <typename T>
    void operator()(const char* n, T* v)
    {
        sizeCfg(n, v);
    }

    template <typename T>
    CfgSizer& operator<<(std::pair<const char*, T*>&& v)
    {
        sizeCfg(v.first, v.second);
        return *this;
    }

    const char* name;
    uint8_t**   cursor;
    int64_t*    size;
    ConfigTree* cfgTree;
};

template <typename T>
size_t GetEncodeSize(T& in)
{
    int64_t         rv = 0;
    CfgProcessor<T> cp;
    cp.size = &rv;
    cp.template process<CfgSizer>("root", &in);
    return rv;
}

#endif // CONFIG_SIZER_HPP
