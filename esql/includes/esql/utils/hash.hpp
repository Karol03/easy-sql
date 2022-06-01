/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <inttypes.h>
#include <utility>

#include "typeof.hpp"


namespace esql::utils
{

class Hash
{
public:
    constexpr static uint32_t hash(const char* data)
    {
        return marmurHash2(data, size(data), 0xCDCDCDCD);
    }

    template <typename T>
    static uint32_t hash(T* data)
    {
        if constexpr (sizeof(T) > 64ul)
        {
            return alder32((const char*)data, sizeof(T));
        }
        else
        {
            return marmurHash2((const char*)data, sizeof(T), 0xCDCDCDCD ^ TypeOf<T>().type());
        }
    }

private:
    constexpr static std::size_t size(const char* data)
    {
        auto result = 0;
        while (data[result] != '\0') ++result;
        return result;
    }

    constexpr static uint32_t marmurHash2(const char* data, std::size_t len, uint32_t seed)
    {
        const unsigned int m = 0x5bd1e995;
        const int r = 24;
        uint32_t h = seed ^ len;

        while (len >= 4)
        {
            unsigned int k = (data[0] << 24) |
                             (data[1] << 16) |
                             (data[2] << 8) |
                             (data[3]);

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            data += 4;
            len -= 4;
        }

        switch(len)
        {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
                h *= m;
        };

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

    constexpr static uint32_t alder32(const char* data, std::size_t len)
    {
        constexpr uint32_t MOD_ADLER = 65521;
        uint32_t a = 1, b = 0;
        std::size_t index{0};

        // Process each byte of the data in order
        for (index = 0; index < len; ++index)
        {
            a = (a + data[index]) % MOD_ADLER;
            b = (b + a) % MOD_ADLER;
        }

        return (b << 16) | a;
    }

};

}  // namespace esql::utils
