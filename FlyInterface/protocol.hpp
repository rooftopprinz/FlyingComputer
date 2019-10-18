#ifndef __FLYINTERFACE_PROTOCOL_HPP__
#define __FLYINTERFACE_PROTOCOL_HPP__

#include <cstdint>
#include <bfc/Buffer.hpp>

namespace flydb
{

enum class MessageType : uint8_t
{
    SetRequest,
    SetResponse,
    SetIndication,
    GetRequest,
    GetResponse
};

using Key = uint8_t;
using TrId = uint8_t;
using Size = uint8_t;

struct Header
{
    MessageType msgType;
    TrId trId;
    Size size;
};

/**********************************
//  Message Definition:
//   
//      Notation:
//          H - header
//          K - key
//          S - size
//          V - value
//   
**********************************/

struct SetRequest : Header {};    // H(KSV)+
struct SetResponse : Header {};   // H
struct SetIndication : Header {}; // H(KSV)+
struct GetRequest : Header {};    // H(K)+
struct GetResponse : Header {};   // H(KSV)+

template <typename>
struct MessageTraits;
template<> struct MessageTraits<SetRequest>    {static constexpr MessageType msgType = MessageType::SetRequest;};
template<> struct MessageTraits<SetResponse>   {static constexpr MessageType msgType = MessageType::SetResponse;};
template<> struct MessageTraits<SetIndication> {static constexpr MessageType msgType = MessageType::SetIndication;};
template<> struct MessageTraits<GetRequest>    {static constexpr MessageType msgType = MessageType::GetRequest;};
template<> struct MessageTraits<GetResponse>   {static constexpr MessageType msgType = MessageType::GetResponse;};

template <typename T>
class Encoder
{
public:
    Encoder(std::byte* pData, size_t pMaxSize)
        : mMsg (new (pData) T{})
        , mData(pData)
        , mCurrent(pData + sizeof(T))
        , mLimit(pData+pMaxSize)
    {
        mMsg->msgType = MessageTraits<T>::msgType;
    }

    T& get()
    {
        return *mMsg;
    }

    template <typename U>
    bool addField(const U& pData)
    {
        if (mCurrent+sizeof(pData) > mLimit)
        {
            return false;
        }

        std::memcpy(mCurrent, &pData, sizeof(pData));

        mCurrent += sizeof(pData);
        mMsg->size = size();

        return true;
    }

    template <typename U>
    bool addField(const Key pKey, const U& pDataIn)
    {
        const Size size = sizeof(pDataIn);

        if (mCurrent+sizeof(pKey)+sizeof(size)+sizeof(pDataIn) > mLimit)
        {
            return false;
        }

        addField(pKey);
        addField(size);
        addField(pDataIn);

        return true;
    }

    bool addField(const Key pKey, std::byte* pRawIn, Size pSize)
    {
        if (mCurrent+sizeof(pKey)+sizeof(pSize)+pSize > mLimit)
        {
            return false;
        }

        addField(pKey);
        addField(pSize);
        std::memcpy(mCurrent, pRawIn, pSize);

        mCurrent += pSize;
        mMsg->size = size();

        return true;
    }

    size_t size()
    {
        return mCurrent - mData;
    }

private:
    T*         mMsg;
    std::byte* mData;
    std::byte* mCurrent;
    std::byte* mLimit;
};

template <typename T>
class Decoder
{
public:
    Decoder(const std::byte* pData, size_t pMaxSize)
        : mData(pData)
        , mCurrent(pData + sizeof(T))
        , mLimit(pData+pMaxSize)
    {}

    T get()
    {
        T rv;
        std::memcpy(&rv, mData, sizeof(T));
        return rv;
    }

    template <typename U>
    bool getField(U& pDataOut)
    {
        if (mCurrent+sizeof(pDataOut) > mLimit)
        {
            return false;
        }

        std::memcpy(&pDataOut, mCurrent, sizeof(pDataOut));
        mCurrent += sizeof(U);
        return true;
    }


    template <typename U>
    bool getField(Key& pKey, U& pDataOut)
    {
        Size size;

        if (mCurrent+sizeof(pKey)+sizeof(size)+sizeof(pDataOut) > mLimit)
        {
            return false;
        }

        getField(pKey);
        getField(size);

        if (size!=sizeof(pDataOut))
        {
            return false;
        }
        getField(pDataOut);
        return true;
    }

    bool getField(Key& pKey, const std::byte* &pRawOut, Size &pSize)
    {
        if (mCurrent+sizeof(pKey)+sizeof(pSize) > mLimit)
        {
            return false;
        }

        getField(pKey);
        getField(pSize);

        if (mCurrent+pSize > mLimit)
        {
            return false;
        }

        pRawOut = mCurrent;
        mCurrent += pSize;
        return true;
    }

private:
    const std::byte* mData;
    const std::byte* mCurrent;
    const std::byte* mLimit;
};

} // namemspace flydb

#endif // __FLYINTERFACE_PROTOCOL_HPP__
