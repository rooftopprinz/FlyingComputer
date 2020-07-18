#ifndef CONFIG_DECODER_HPP
#define CONFIG_DECODER_HPP
/******************************************************************************
*       Copyright Nokia 2019
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*       The code contains Nokia Confidential Proprietary Information.
*
******************************************************************************/
#include "ConfigProcessor.hpp"

struct CfgDecoder : public CodecBase
{
    CfgDecoder(const char* name, uint8_t** cursor, int64_t* size, ConfigTree* cfgTree)
        : CodecBase(cursor, size)
        , name(name)
    {
    }

    ~CfgDecoder()
    {
    }

    template <typename T>
    void unalignedStore(T& value)
    {
        std::memcpy(&value, *cursor, sizeof(T));
    }

    template <typename T>
    typename EnableIfNotCfgConstructible<T>::type decodeCfg(const char* name, std::vector<T>* va)
    {
        checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, sizeof(uint32_t));
        uint32_t ecount;
        unalignedStore(ecount);
        updateSize(sizeof(uint32_t));
        checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, ecount * sizeof(T));
        va->assign((T*)(*cursor), (T*)(*cursor + ecount * sizeof(T)));
        updateSize(ecount * sizeof(T));
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type decodeCfg(const char* name, std::vector<T>* va)
    {
        va->clear();
        uint32_t ecount;
        unalignedStore(ecount);
        updateSize(sizeof(uint32_t));
        for (uint32_t i = 0; i < ecount; i++)
        {
            va->emplace_back();
            CfgProcessor<T> cp;
            cp.cursor = cursor;
            cp.size   = size;
            cp.template process<CfgDecoder>(name, &va->back());
        }
    }

    template <typename T>
    typename EnableIfNotCfgConstructible<T>::type decodeCfg(const char* name, T* va)
    {
        checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, sizeof(T));
        unalignedStore(*va);
        updateSize(sizeof(T));
    }

    void decodeCfg(const char* name, std::string* va)
    {
        checkBounds(name, __FILE__, __PRETTY_FUNCTION__, __LINE__, std::strlen((const char*)*cursor) + 1); // still unsafe
        *va = (const char*)*cursor;
        updateSize(va->size() + 1);
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type decodeCfg(const char* name, T* va)
    {
        CfgProcessor<T> cp;
        cp.cursor = cursor;
        cp.size   = size;
        cp.template process<CfgDecoder>(name, va);
    }

    template <typename T>
    void operator()(const char* n, T* v)
    {
        decodeCfg(n, v);
    }

    template <typename T>
    CfgDecoder& operator<<(std::pair<const char*, T*>&& v)
    {
        decodeCfg(v.first, v.second);
        return *this;
    }

    const char* name;
    ConfigTree* cfgTree;
};

template <typename T>
size_t DecodeConfig(T& out, uint8_t* in, int64_t limit)
{
    uint8_t*        begin = in;
    CfgProcessor<T> cp;
    cp.size   = &limit;
    cp.cursor = &begin;
    cp.template process<CfgDecoder>("root", &out);
    return begin - in;
}

#endif // CONFIG_DECODER_HPP
