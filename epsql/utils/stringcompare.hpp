/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once


namespace epsql::utils
{

inline constexpr bool startsWith(const char* str, const char* begin)
{
    auto i = 0u;
    for (; begin[i] != '\0' && str[i] != '\0'; ++i)
        if (begin[i] != str[i])
            return false;
    return begin[i] == '\0';
}

}  // namespace epsql::utils
