#ifndef CONFIG_PRINTER_HPP
#define CONFIG_PRINTER_HPP
/******************************************************************************
*       Copyright Nokia 2019
*
*       The copyright notice above does not evidence any
*       actual or intended publication of such source code.
*       The code contains Nokia Confidential Proprietary Information.
*
******************************************************************************/
#include "ConfigProcessor.hpp"

#include <regex>
#include <sstream>

struct CfgPrinter
{
    CfgPrinter(const char* name, uint8_t** cursorInDisguise, int64_t* size, ConfigTree* cfgTree)
        : name(name)
        , size(size)
        , ss(*(std::stringstream*)(cursorInDisguise))
    {
    }

    ~CfgPrinter()
    {
    }

    template <typename T>
    typename EnableIfNotCfgConstructible<T>::type printCfg(const char* name, std::vector<T>* va)
    {
        ss << "\"" << name << "\":[";
        for (const auto i : *va)
        {
            ss << i << ",";
        }
        ss << "],";
    }
    void printCfg(const char* name, std::vector<uint8_t>* va)
    {
        ss << "\"" << name << "\":[";
        for (const auto i : *va)
        {
            ss << unsigned(i) << ",";
        }
        ss << "],";
    }

    void printCfg(const char* name, std::vector<int8_t>* va)
    {
        ss << "\"" << name << "\":[";
        for (const auto i : *va)
        {
            ss << signed(i) << ",";
        }
        ss << "],";
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type printCfg(const char* name, std::vector<T>* va)
    {
        ss << "\"" << name << "\":[";
        for (T& i : *va)
        {
            ss << "{";
            CfgProcessor<T> cp;
            cp.size   = size;
            cp.cursor = (uint8_t**)&ss;
            cp.template process<CfgPrinter>(name, &i);
            ss << "},";
        }
        ss << "],";
    }

    template <typename T>
    typename EnableIfNotCfgConstructible<T>::type printCfg(const char* name, T* va)
    {
        ss << "\"" << name << "\":" << *va << ",";
    }

    void printCfg(const char* name, uint8_t* va)
    {
        ss << "\"" << name << "\":" << unsigned(*va) << ",";
    }

    void printCfg(const char* name, int8_t* va)
    {
        ss << "\"" << name << "\":" << signed(*va) << ",";
    }

    void printCfg(const char* name, std::string* va)
    {
        ss << "\"" << name << "\":\"" << *va << "\",";
    }

    template <typename T>
    typename EnableIfCfgConstructible<T>::type printCfg(const char* name, T* va)
    {
        ss << "\"" << name << "\":{";
        CfgProcessor<T> cp;
        cp.cursor = (uint8_t**)&ss;
        cp.size   = size;
        cp.template process<CfgPrinter>(name, va);
        ss << "},";
    }

    template <typename T>
    void operator()(const char* n, T* v)
    {
        printCfg(n, v);
    }

    template <typename T>
    CfgPrinter& operator<<(std::pair<const char*, T*>&& v)
    {
        printCfg(v.first, v.second);
        return *this;
    }

    const char*        name;
    uint8_t**          cursor;
    int64_t*           size;
    ConfigTree*        cfgTree;
    std::stringstream& ss;
};

template <typename T>
std::string GetPrintedString(T& in)
{
    std::stringstream ss;
    ss << "{";
    CfgProcessor<T> cp;
    cp.cursor = (uint8_t**)&ss;
    cp.template process<CfgPrinter>("root", &in);
    ss << "}";
    return std::regex_replace(ss.str(), std::regex(",(\\]|\\})"), "$1");
}

#endif // CONFIG_PRINTER_HPP
