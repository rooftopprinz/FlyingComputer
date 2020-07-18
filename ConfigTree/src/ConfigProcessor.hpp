#ifndef CONFIG_PROCESSOR_HPP
#define CONFIG_PROCESSOR_HPP
/******************************************************************************
*       Copyright Nokia 2019
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*       The code contains Nokia Confidential Proprietary Information.
*
******************************************************************************/
#include "ConfigTree.hpp"

struct CfgProcessorBase
{
    CfgProcessorBase()  = default;
    uint8_t**   cursor  = nullptr;
    int64_t*    size    = nullptr;
    ConfigTree* cfgTree = nullptr;
};

struct CodecBase
{
    CodecBase(uint8_t** cursor, int64_t* size)
        : cursor(cursor)
        , size(size)
    {
    }

    void checkBounds(const char* name, const char* file, const char* func, int line, int64_t reqSize)
    {
        if (reqSize > *size)
        {
            throw std::runtime_error(std::string("Encode/Decode out of bounds! check: in field: ") + name + " function: " + func + " in file: " +
                                     file + ":" + std::to_string(line));
        }
    }

    inline void updateSize(size_t rsize)
    {
        *cursor += rsize;
        *size -= rsize;
    }
    uint8_t** cursor;
    int64_t*  size;
};

template <typename T>
struct CfgProcessor : public CfgProcessorBase
{
    CfgProcessor() = delete;
};

template <>
/** Note: Treat std::string as NPOD **/
struct CfgProcessor<std::string> : public CfgProcessorBase
{
    template <typename Doer>
    void process(const char* n, std::string* v)
    {
        Doer doit(".", cursor, size, cfgTree);
        doit(n, v);
    }
};

template <typename T>
struct IsCfgConstructible : std::is_constructible<CfgProcessor<T>>
{
};
template <typename T>
struct EnableIfCfgConstructible : std::enable_if<IsCfgConstructible<T>::value>
{
};
template <typename T>
struct EnableIfNotCfgConstructible : std::enable_if<!IsCfgConstructible<T>::value>
{
};

#endif // CONFIG_PROCESSOR_HPP
