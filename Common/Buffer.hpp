#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <type_traits>
#include <cstddef>
#include <cstring>

namespace std
{
    enum class byte : uint8_t;
}
namespace common
{

class Buffer
{
public:
    Buffer() = default;
    Buffer(std::byte* pData, size_t pSize, bool pAssumeOwnership=true)
        : mSize(pSize)
        , mData(pData)
        , mIsOwned(pAssumeOwnership)
    {
    }

    Buffer(const Buffer&) = delete;
    void operator=(const Buffer&) = delete;

    ~Buffer()
    {
        reset();
    }

    Buffer(Buffer&& pOther) noexcept
    {
        reset();
        transferOwnership(std::move(pOther));
    }

    Buffer& operator =(Buffer&& pOther) noexcept
    {
        reset();
        transferOwnership(std::move(pOther));
        return *this;
    }

    std::byte* data() noexcept
    {
        return mData;
    }

    const std::byte* data() const noexcept
    {
        return mData;
    }

    size_t size() const noexcept
    {
        return mSize;
    }

    bool isOwned() const noexcept
    {
        return mIsOwned;
    }

    Buffer copy()
    {
        if (!mSize)
        {
            return Buffer();
        }

        Buffer rv(new std::byte[mSize], mSize);
        std::memcpy(rv.mData, mData, mSize);
        return rv;
    }

    void reset() noexcept
    {
        if (mData && mIsOwned)
            delete[] mData;
        clear(std::move(*this));
    }

private:
    static void clear(Buffer&& pOther) noexcept
    {
        pOther.mData = nullptr;
        pOther.mSize = 0;
        pOther.mIsOwned = false;
    }

    void transferOwnership(Buffer&& pOther) noexcept
    {
        mData = pOther.mData;
        mSize = pOther.mSize;
        mIsOwned = pOther.mIsOwned;
        clear(std::move(pOther));
    };

    size_t mSize = 0;
    std::byte* mData = nullptr;
    bool mIsOwned = false;
};

} // namespace common
#endif // __BUFFER_HPP__