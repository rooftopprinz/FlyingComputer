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

template<typename T, size_t N>
class static_array
{
public:
    static_array() = default;

    ~static_array()
    {
        clear();
    }

    static_array(const static_array& pOther)
    {
        for (auto& i: pOther)
        {
            emplace(i);
        }
    }

    static_array(static_array&& pOther)
    {
        for (auto& i: pOther)
        {
            emplace_back(std::move(i));
        }
        pOther.clear();
    }

    static_array& operator=(const static_array& pOther)
    {
        clear();
        for (auto& i: pOther)
        {
            emplace_back(i);
        }
        return *this;
    }

    static_array& operator=(static_array&& pOther)
    {
        clear();
        for (auto& i: pOther)
        {
            emplace_back(std::move(i));
        }
        pOther.clear();
        return *this;
    }

    void clear()
    {
        size_t oSize = mSize;
        for (size_t i=0; i<oSize; i++)
        {
            pop();
        }
    }

    static_array(const std::initializer_list<T>& pList)
    {
        for (auto& i : pList)
        {
            emplace_back(i);
        }
    }

    static_array& operator=(const std::initializer_list<T>& pList)
    {
        clear();
        for (auto& i : pList)
        {
            emplace_back(i);
        }
        return *this;
    }

    template <typename... U>
    T& emplace_back(U&&... pArgs)
    {
        if (mSize >= N)
        {
            throw std::out_of_range("trying to emplace when size() >= N");
        }
        new ((T*)mData+mSize) T(std::forward<U>(pArgs)...);
        mSize++;
        return back();
    }

    T& operator[](size_t pIndex)
    {
        return ((T*)mData)[pIndex];
    }

    const T& operator[](size_t pIndex) const
    {
        return ((T*)mData)[pIndex];
    }

    void pop()
    {
        (*this)[--mSize].~T();
    }

    T* begin()
    {
        return (T*)mData;
    }

    T* end()
    {
        return ((T*)mData)+mSize;
    }

    const T* begin() const
    {
        return (T*)mData;
    }

    const T* end() const
    {
        return ((T*)mData)+mSize;
    }

    T& front()
    {
        return *begin();
    }

    T& back()
    {
        return *(end()-1);
    }

    const T& front() const
    {
        return *begin();
    }

    const T& back() const
    {
        return *(end()-1);
    }

    const T* cbegin()
    {
        return (T*)mData;
    }

    const T* cend()
    {
        return ((T*)mData)+mSize;
    }

    const T* cbegin() const
    {
        return (T*)mData;
    }

    const T* cend() const
    {
        return ((T*)mData)+mSize;
    }


    size_t size() const
    {
        return mSize;
    }

private:
    size_t mSize = 0;
    uint8_t mData[sizeof(T)*N];
};

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
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": not enough encode buffer.");
    std::memcpy(pCtx.get(), pIeOctet, pSize);
    pCtx.advance(pSize);
}

inline void decode_per(uint8_t *pIeOctet, size_t pSize, per_codec_ctx& pCtx)
{
    if (pSize > pCtx.size())
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": decode attempted past end of buffer.");
    std::memcpy(pIeOctet, pCtx.get(), pSize);
    pCtx.advance(pSize);
}

template <typename T>
inline void encode_per(const T pIe, per_codec_ctx& pCtx)
{
    if (sizeof(pIe) > pCtx.size())
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": not enough encode buffer.");
    new (pCtx.get()) decltype(pIe)(pIe);
    pCtx.advance(sizeof(pIe));
}

template <typename T>
inline void decode_per(T& pIe, per_codec_ctx& pCtx)
{
    if (sizeof(pIe) > pCtx.size())
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": decode attempted past end of buffer.");
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
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": not enough encode buffer.");
    std::memcpy(pCtx.get(), pIe.data(), strsz);
    pCtx.advance(strsz);
}

inline void decode_per(std::string& pIe, per_codec_ctx& pCtx)
{
    // TODO: safer pls
    pIe = (const char*)pCtx.get();
    const size_t strsz = pIe.size()+1;
    if (strsz > pCtx.size())
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": decode attempted past end of buffer.");
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
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": not enough encode buffer.");
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
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": decode attempted past end of buffer.");
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


template <typename T, size_t N>
void encode_per(const static_array<T, N>& pIe, size_t pIndexSize, per_codec_ctx& pCtx)
{
    if (pIndexSize > pCtx.size())
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": not enough encode buffer.");
    size_t size = pIe.size();
    encode_per((uint8_t*)&size, pIndexSize, pCtx);
    for (auto& i : pIe)
    {
        encode_per(i, pCtx);
    }
}

template <typename T, size_t N>
void decode_per(static_array<T, N>& pIe, size_t pIndexSize, per_codec_ctx& pCtx)
{
    if (pIndexSize > pCtx.size())
    {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": decode attempted past end of buffer.");
    }
    size_t size = 0;
    decode_per((uint8_t*)&size, pIndexSize, pCtx);
    if (size>N)
    {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__)+": decode attempted past end of buffer.");
    }
    for (size_t i=0u; i<size; i++)
    {
        pIe.emplace_back();
        decode_per(pIe.back(), pCtx);
    }
}

template <typename T, size_t N>
void str(const char* pName, const static_array<T, N>& pIe, std::string& pCtx, bool pIsLast)
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