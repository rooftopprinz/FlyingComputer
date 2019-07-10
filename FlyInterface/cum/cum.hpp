#ifndef __CUM_HPP__
#define __CUM_HPP__

#include <type_traits>
#include <stdexcept>
#include <variant>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

namespace cum
{

class per_codec_ctx
{
public:
    enum class coder {uper, per};
    per_codec_ctx(std::byte* pData, size_t pSize)
        : mData(pData)
        , mSize(pSize)
    {}

    std::byte* get()
    {
        return mData;
    }

    void advance(size_t pSize)
    {
        mData += pSize;
        mSize -= pSize;
    }

    size_t size() const
    {
        return mSize;
    }

private:
    std::byte* mData;
    size_t mSize;
};

inline void encode_per(const uint8_t *pIeOctet, size_t pSize, per_codec_ctx& pCtx)
{
    if (pSize > pCtx.size())
        throw std::out_of_range(__PRETTY_FUNCTION__);
    std::memcpy(pCtx.get(), pIeOctet, pSize);
    pCtx.advance(pSize);
}

inline void decode_per(uint8_t *pIeOctet, size_t pSize, per_codec_ctx& pCtx)
{
    if (pSize > pCtx.size())
        throw std::out_of_range(__PRETTY_FUNCTION__);
    std::memcpy(pIeOctet, pCtx.get(), pSize);
    pCtx.advance(pSize);
}

template <typename T>
inline void encode_per(const T pIe, per_codec_ctx& pCtx)
{
    if (sizeof(pIe) > pCtx.size())
        throw std::out_of_range(__PRETTY_FUNCTION__);
    new (pCtx.get()) decltype(pIe)(pIe);
    pCtx.advance(sizeof(pIe));
}

template <typename T>
inline void decode_per(T& pIe, per_codec_ctx& pCtx)
{
    if (sizeof(pIe) > pCtx.size())
        throw std::out_of_range(__PRETTY_FUNCTION__);
    std::memcpy(&pIe, pCtx.get(), sizeof(pIe));
    pCtx.advance(sizeof(pIe));
}

template <typename T>
inline void str(const char* pName, const T& pIe, std::string& pCtx, bool isLast)
{
    if (!pName)
    {
        pCtx = pCtx + "\"" + std::to_string(pIe) + "\"";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":\"" + std::to_string(pIe) + "\"";
    }

    if (!isLast)
    {
        pCtx += ",";
    }
}

inline void str(const char* pName, const char pIe, std::string& pCtx, bool isLast)
{
    if (!pName)
    {
        pCtx = pCtx + "\"" + pIe + "\"";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":\"" + std::to_string(pIe) + "\"";
    }

    if (!isLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const std::string& pIe, per_codec_ctx& pCtx)
{
    const size_t strsz = pIe.size()+1;
    if (strsz > pCtx.size())
        throw std::out_of_range(__PRETTY_FUNCTION__);
    std::memcpy(pCtx.get(), pIe.data(), strsz);
    pCtx.advance(strsz);
}

inline void decode_per(std::string& pIe, per_codec_ctx& pCtx)
{
    // TODO: safer pls
    pIe = (const char*)pCtx.get();
    const size_t strsz = pIe.size()+1;
    if (strsz > pCtx.size())
        throw std::out_of_range(__PRETTY_FUNCTION__);
    pCtx.advance(strsz);
}

inline void str(const char* pName, const std::string& pIe, std::string& pCtx, bool isLast)
{
    if (!pName)
    {
        pCtx = pCtx + "\"" + pIe + "\"";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":\"" + pIe + "\"";
    }

    if (!isLast)
    {
        pCtx += ",";
    }
}

template <typename T>
void encode_per(const std::vector<T>& pIe, size_t pIndexSize, per_codec_ctx& pCtx)
{
    if (pIndexSize > pCtx.size())
        throw std::out_of_range(__PRETTY_FUNCTION__);
    size_t size = pIe.size();
    encode_per((uint8_t*)&size, pIndexSize, pCtx);
    for (auto& i : pIe)
    {
        encode_per(i, pCtx);
    }
}

template <typename T>
void decode_per(std::vector<T>& pIe, size_t pIndexSize, per_codec_ctx& pCtx)
{
    if (pIndexSize > pCtx.size())
    {
        throw std::out_of_range(__PRETTY_FUNCTION__);
    }
    size_t size = 0;
    decode_per((uint8_t*)&size, pIndexSize, pCtx);
    for (size_t i=0u; i<size; i++)
    {
        pIe.emplace_back();
        decode_per(pIe.back(), pCtx);
    }
}

template <typename T>
void str(const char* pName, const std::vector<T>& pIe, std::string& pCtx, bool pIsLast)
{
    if (!pName)
    {
        pCtx = pCtx + "[";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":[";
    }
    for (size_t i=0; i<pIe.size();i++)
    {
        str(nullptr, pIe[i], pCtx, (i>=pIe.size()-1) ? true : false);
    }
    pCtx += "]";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline bool check_optional(uint8_t *pOptionalMask, size_t n)
{
    size_t opos = n >> 3;
    size_t bpos = n & 7;
    return pOptionalMask[opos] & (0x80u >> bpos);
}

inline void set_optional(uint8_t *pOptionalMask, size_t n)
{
    size_t opos = n >> 3;
    size_t bpos = n & 7;
    pOptionalMask[opos] |= (0x80u >> bpos);
}

} // namespace cum
#endif // __CUM_HPP__