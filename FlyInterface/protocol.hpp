#ifndef __FLYINTERFACE_PROTOCOL_HPP__
#define __FLYINTERFACE_PROTOCOL_HPP__

#include <cstdint>
#include <Buffer.hpp>

namespace flydb
{

enum class MessageTypes : uint8_t
{
    SetRequest,
    SetResponse,
    SetIndication,
    GetRequest,
    GetResponse
};

using MessageType = uint8_t;
using Key = uint8_t;
using TrId = uint8_t;
using Size = uint8_t;

struct Header
{
    MessageType msgType;
    TrId trId;
};

struct SetRequest
{
    Header header;
    Size size;
};

struct SetResponse
{
    Header header;
};

struct SetIndication
{
    Header header;
    Size size;
};

struct GetRequest
{
    Header header;
    Size size;
};

struct GetResponse
{
    Header header;
    Size size;
};

template <typename>
struct MessageTraits;
template struct MessageTraits<SetRequest>    {constexpr MessageTypes msgType = MessageTypes::SetRequest;};
template struct MessageTraits<SetResponse>   {constexpr MessageTypes msgType = MessageTypes::SetResponse;};
template struct MessageTraits<SetIndication> {constexpr MessageTypes msgType = MessageTypes::SetIndication;};
template struct MessageTraits<GetRequest>    {constexpr MessageTypes msgType = MessageTypes::GetRequest;};
template struct MessageTraits<GetResponse>   {constexpr MessageTypes msgType = MessageTypes::GetResponse;};

template <typename T>
class Encoder
{
public:
    Encoder(std::byte* pData, size_t pMaxSize)
        : mData(pData)
        , mMaxSize(pSize)
    {}

    T* initialize()
    {
        auto rv = new (mData) T{};
        rv->header.msgType = MessageTraits<T>::msgType;
        mCurrent = mData + sizeof(T);
    }

    template <typename T>
    void addField(const T& pData)
    {
        std::memcpy(mCurrent, &pData, sizeof(pData));
        mCurrent += sizeof(pData);
    }

    template <typename T>
    void addField(const Key pKey, const T& pDataIn)
    {
        addField(pKey);
        addField((Size)sizeof(pDataIn));
        addField(pDataIn);
    }

    void addField(const Key pKey, std::byte* pRawIn, Size pSize)
    {
        addField(pKey);
        addField(pSize);
        std::memcpy(mCurrent, pRawIn, pSize)
        mCurrent += pSize;
    }

    size_t useSize()
    {
        return mCurrent - mData;
    }

private:
    std::byte* mData;
    std::byte* mCurrent;
    size_t mMaxSize;
};

template <typename T>
class Decoder
{
public:
    Decoder(std::byte* pData, size_t pMaxSize)
        : mData(pData)
        , mMaxSize(pSize)
    {}

    T get()
    {
        T rv;
        std::memcpy(&rv, mData, sizeof(T));
        return rv;
    }

    template <typename T>
    void getField(T& pDataOut)
    {
        std::memcpy(&pDataOut, mCurrent, sizeof(pDataOut));
        mCurrent += sizeof(T);
    }

    template <typename T>
    void getField(Key& pKey, T& pDataOut)
    {
        uint8_t size;

        (pKey);
        getField(size);

        if (sizeof(T)!=size)
        {
            return nullptr;
        }

        getAndAdvance(pDataOut);

        return pRawIn;
    }

    void getField(Key& pKey, std::byte* &pRawOut, Size &pSize)
    {
        getField(pKey);
        getField(pSize);
        std::memcpy(pRawOut, mCurrent, pSize)
        return mCurrent + pSize;
    }
private:
    std::byte* mData;
    std::byte* mCurrent;
    size_t mMaxSize;
};

} // namemspace flydb

#endif // __FLYINTERFACE_PROTOCOL_HPP__
