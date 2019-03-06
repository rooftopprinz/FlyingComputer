#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <thread>
#include <chrono>

using BufferLog = std::pair<uint16_t, void*>;

#define _ static constexpr
template<typename> struct TypeTraits;
template<> struct TypeTraits<unsigned char>      {_ auto type_id = 0xa1; _ size_t size =  sizeof(unsigned char);      _ char cfmt[] = "%c";};
template<> struct TypeTraits<signed char>        {_ auto type_id = 0xa2; _ size_t size =  sizeof(signed char);        _ char cfmt[] = "%c";};
template<> struct TypeTraits<unsigned short>     {_ auto type_id = 0xa3; _ size_t size =  sizeof(unsigned short);     _ char cfmt[] = "%u";};
template<> struct TypeTraits<short>              {_ auto type_id = 0xa4; _ size_t size =  sizeof(short);              _ char cfmt[] = "%d";};
template<> struct TypeTraits<unsigned int>       {_ auto type_id = 0xa5; _ size_t size =  sizeof(unsigned int);       _ char cfmt[] = "%u";};
template<> struct TypeTraits<int>                {_ auto type_id = 0xa6; _ size_t size =  sizeof(int);                _ char cfmt[] = "%d";};
template<> struct TypeTraits<unsigned long>      {_ auto type_id = 0xa7; _ size_t size =  sizeof(unsigned long);      _ char cfmt[] = "%ld";};
template<> struct TypeTraits<long>               {_ auto type_id = 0xa8; _ size_t size =  sizeof(long);               _ char cfmt[] = "%lu";};
template<> struct TypeTraits<unsigned long long> {_ auto type_id = 0xa9; _ size_t size =  sizeof(unsigned long long); _ char cfmt[] = "%lld";};
template<> struct TypeTraits<long long>          {_ auto type_id = 0xaa; _ size_t size =  sizeof(long long);          _ char cfmt[] = "%llu";};
template<> struct TypeTraits<float>              {_ auto type_id = 0xab; _ size_t size =  sizeof(float);              _ char cfmt[] = "%f";};
template<> struct TypeTraits<double>             {_ auto type_id = 0xac; _ size_t size =  sizeof(double);             _ char cfmt[] = "%f";};
template<> struct TypeTraits<void*>              {_ auto type_id = 0xad; _ size_t size =  sizeof(void*);              _ char cfmt[] = "%p";};
template<> struct TypeTraits<BufferLog>          {_ auto type_id = 0xae; _ size_t size = 0;                           _ char cfmt[] = "%s";};
template<> struct TypeTraits<const char*>        {_ auto type_id = 0xaf; _ size_t size = 0;                           _ char cfmt[] = "%p";};
#undef _

template <typename... Ts>
struct TotalSize
{
    static constexpr size_t value = (TypeTraits<Ts>::size+...);
};

template<>
struct TotalSize<>
{
    static constexpr size_t value = 0;
};


inline std::string toHexString(const uint8_t* pData, size_t size)
{
    std::stringstream ss;;
    for (size_t i=0; i<size; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << unsigned(pData[i]);
    }

    return ss.str();
}

class Logger
{
public:
    using HeaderType = int32_t;
    using TagType    = uint8_t;
    using TailType   = uint8_t;
    template<typename... Ts>
    void log(const char * id, uint64_t pTime, uint64_t pThread, const Ts&... ts)
    {
        if (mLogful)
        {
            uint8_t logbuff[4096];
            int flen = std::sprintf((char*)logbuff, "%lluus %llut ", (unsigned long long)pTime, (unsigned long long)pThread);
            size_t sz = logful(logbuff + flen, id, ts...) + flen;
            logbuff[sz++] = '\n';
            ::write(1, logbuff, sz);
        }
        {       
            // constexpr size_t payloadSize = sizeof(HeaderType) + sizeof(TagType)*2 + sizeof(pTime) + sizeof(pThread) +
                // sizeof(TagType)*sizeof...(Ts) + TotalSize<Ts...>::value + sizeof(TailType);
            uint8_t usedBuffer[2048];
            int usedIdx = 0;
            new (usedBuffer + usedIdx) HeaderType(intptr_t(id)-intptr_t(LoggerRef));
            usedIdx += sizeof(HeaderType);
            size_t sz = logless(usedBuffer, usedIdx, pTime, pThread, ts...) + sizeof(HeaderType);
            std::fwrite((char*)usedBuffer, 1, sz, mOutputFile);
        }
        flush();
    }
    void logful()
    {
        mLogful = true;
    }
    void logless()
    {
        mLogful = false;
    }
    void flush()
    {
        std::fflush(mOutputFile);
    }
    static Logger& getInstance()
    {
        static Logger logger{};
        return logger;
    }
private:

    Logger()
        : mOutputFile(std::fopen("log.bin", "wb"))
    {
        std::cout << "Logger::Logger\n";
    }

    ~Logger()
    {
        std::cout << "Logger::~Logger\n";
        std::fclose(mOutputFile);
    }

    const char* findNextToken(char pTok, const char* pStr)
    {
        while (*pStr!=0&&*pStr!=pTok)
        {
            pStr++;
        }
        return pStr;
    }

    size_t logful(uint8_t* pOut, const char* pMsg)
    {
        const char *nTok = findNextToken('_',pMsg);
        size_t sglen = uintptr_t(nTok)-uintptr_t(pMsg);
        std::memcpy(pOut, pMsg, sglen);
        return sglen;
    }

    template<typename T, typename... Ts>
    size_t logful(uint8_t* pOut, const char* pMsg, T t, Ts... ts)
    {
        const char *nTok = findNextToken('_',pMsg);
        size_t sglen = uintptr_t(nTok)-uintptr_t(pMsg);
        std::memcpy(pOut, pMsg, sglen);
        pMsg+=sglen;
        pOut+=sglen;
        int flen = 0;
        if (*nTok)
        {
            flen = std::sprintf((char*)pOut, TypeTraits<T>::cfmt, t);
            pMsg++;
        }
        if (flen>0) pOut += flen;
        return sglen + flen + logful(pOut, pMsg, ts...);
    }

    template<typename... Ts>
    size_t logful(uint8_t* pOut, const char* pMsg, BufferLog t, Ts... ts)
    {
        const char *nTok = findNextToken('_',pMsg);
        size_t sglen = uintptr_t(nTok)-uintptr_t(pMsg);
        std::memcpy(pOut, pMsg, sglen);
        pMsg+=sglen;
        pOut+=sglen;
        int flen = 0;
        if (*nTok)
        {
            auto s = toHexString((uint8_t*)t.second, t.first);
            std::memcpy(pOut, s.data(), s.size());
            flen += s.size();
            pMsg++;
        }
        if (flen>0) pOut += flen;
        return sglen + flen + logful(pOut, pMsg, ts...);
    }

    template<typename... Ts>
    size_t logful(uint8_t* pOut, const char* pMsg, const char *t, Ts... ts)
    {
        const char *nTok = findNextToken('_',pMsg);
        size_t sglen = uintptr_t(nTok)-uintptr_t(pMsg);
        std::memcpy(pOut, pMsg, sglen);
        pMsg+=sglen;
        pOut+=sglen;
        int flen = 0;
        if (*nTok)
        {
            auto s = std::string_view(t, std::strlen(t));
            std::memcpy(pOut, s.data(), s.size());
            flen += s.size();
            pMsg++;
        }
        if (flen>0) pOut += flen;
        return sglen + flen + logful(pOut, pMsg, ts...);
    }

    size_t logless(uint8_t* pUsedBuffer, int& pUsedIndex)
    {
        new (pUsedBuffer+pUsedIndex) TailType(0);
        return sizeof(TailType);
    }

    template<typename T, typename... Ts>
    size_t logless(uint8_t* pUsedBuffer, int& pUsedIndex, T t, Ts... ts)
    {
        new (pUsedBuffer + pUsedIndex) TagType(TypeTraits<T>::type_id);
        pUsedIndex += sizeof(TagType);
        new (pUsedBuffer + pUsedIndex) T(t);
        pUsedIndex += sizeof(T);
        return logless(pUsedBuffer, pUsedIndex, ts...) + sizeof(TagType) + sizeof(T);
    }

    template<typename... Ts>
    size_t logless(uint8_t* pUsedBuffer, int& pUsedIndex, BufferLog t, Ts... ts)
    {
        new (pUsedBuffer + pUsedIndex) TagType(TypeTraits<BufferLog>::type_id);
        pUsedIndex += sizeof(TagType);
        new (pUsedBuffer + pUsedIndex) BufferLog::first_type(t.first);
        pUsedIndex += sizeof(BufferLog::first_type);
        std::memcpy(pUsedBuffer + pUsedIndex, t.second, t.first);
        pUsedIndex += t.first;
        return logless(pUsedBuffer, pUsedIndex, ts...) + sizeof(TagType) + sizeof(BufferLog::first_type) + t.first;
    }

    template<typename... Ts>
    size_t logless(uint8_t* pUsedBuffer, int& pUsedIndex, const char* t, Ts... ts)
    {
        new (pUsedBuffer + pUsedIndex) TagType(TypeTraits<const char*>::type_id);
        pUsedIndex += sizeof(TagType);
        size_t tlen = strlen(t);
        new (pUsedBuffer + pUsedIndex) BufferLog::first_type(tlen);
        pUsedIndex += sizeof(BufferLog::first_type);
        std::memcpy(pUsedBuffer + pUsedIndex, t, tlen);
        pUsedIndex += tlen;
        return logless(pUsedBuffer, pUsedIndex, ts...) + sizeof(TagType) + sizeof(BufferLog::first_type) + tlen;
    }


    std::FILE* mOutputFile;
    bool mLogful = false;
    static const char* LoggerRef;
};

template <typename... Ts>
void Logless(const char* id, Ts... ts)
{
    uint64_t timeNow = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    uint64_t threadId = std::hash<std::thread::id>()(std::this_thread::get_id());
    Logger::getInstance().log(id, timeNow, threadId, ts...);
}

#endif // __LOGGER_HPP__