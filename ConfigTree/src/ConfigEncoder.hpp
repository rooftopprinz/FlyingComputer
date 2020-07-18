#ifndef CONFIG_ENCODER_HPP
#define CONFIG_ENCODER_HPP
/******************************************************************************
*       Copyright Nokia 2019
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*       The code contains Nokia Confidential Proprietary Information.
*
******************************************************************************/
#include "ConfigProcessor.hpp"

struct CfgEncoder : public CodecBase
{
    CfgEncoder(const char* name, uint8_t** cursor, int64_t* size, ConfigTree* cfgTree)
        : CodecBase(cursor, size)
        , name(name)
    {
    }

    ~CfgEncoder()
    {
    }

    template <typename T>
    void unalignedStore(T value)
    {
        std::memcpy(*cursor, &value, sizeof(T));
    }

    template <typename T>
    typename EnableIfNotCfgConstructible<T>::type encodeCfg(const char* name, std::vector<T>* va)
    {
        checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, sizeof(uint32_t));
        unalignedStore(va->size());
        updateSize(sizeof(uint32_t));
        for (const T& i : *va)
        {
            checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, sizeof(T));
            *(T*)(*cursor) = i;
            updateSize(sizeof(T));
        }
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type encodeCfg(const char* name, std::vector<T>* va)
    {
        checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, sizeof(uint32_t));
        unalignedStore(va->size());
        updateSize(sizeof(uint32_t));
        for (T& i : *va)
        {
            CfgProcessor<T> cp;
            cp.cursor = cursor;
            cp.size   = size;
            cp.template process<CfgEncoder>(name, &i);
        }
    }

    template <typename T>
    typename EnableIfNotCfgConstructible<T>::type encodeCfg(const char* name, T* va)
    {
        checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, sizeof(T));
        unalignedStore(*va);
        updateSize(sizeof(T));
    }

    void encodeCfg(const char* name, std::string* va)
    {
        checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, va->size() + 1);
        std::memcpy(*cursor, va->c_str(), va->size() + 1);
        updateSize(va->size() + 1);
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type encodeCfg(const char* name, T* va)
    {
        CfgProcessor<T> cp;
        cp.cursor = cursor;
        cp.size   = size;
        cp.template process<CfgEncoder>(name, va);
    }

    template <typename T>
    void operator()(const char* n, T* v)
    {
        encodeCfg(n, v);
    }

    template <typename T>
    CfgEncoder& operator<<(std::pair<const char*, T*>&& v)
    {
        encodeCfg(v.first, v.second);
        return *this;
    }

    const char* name;
    ConfigTree* cfgTree;
};

template <typename T>
size_t EncodeConfig(T& in, uint8_t* out, int64_t limit)
{
    uint8_t*        begin = out;
    CfgProcessor<T> cp;
    cp.size   = &limit;
    cp.cursor = &begin;
    cp.template process<CfgEncoder>("root", &in);
    return begin - out;
}

#endif // CONFIG_ENCODER_HPP
