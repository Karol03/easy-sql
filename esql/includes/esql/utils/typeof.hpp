/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "esql/db/table/types.hpp"


namespace esql::utils
{

struct ITypeOf
{
protected:
    static inline uint64_t generate() noexcept
    {
        static uint64_t i = 0;
        return ++i;
    }
};


template <typename T>
struct TypeOf final : public ITypeOf
{
public:
    constexpr TypeOf() noexcept
    {
        if (!m_type)
            m_type = ITypeOf::generate();
    }

    constexpr inline auto type() noexcept
    {
        return m_type;
    }

private:
    static inline uint64_t m_type = 0;
};


template <typename T>
inline constexpr bool isNumeric()
{
    return std::is_same_v<T, Smallint> ||
           std::is_same_v<T, Int> ||
           std::is_same_v<T, Bigint> ||
           std::is_same_v<T, Real> ||
           std::is_same_v<T, Float8>;
}

inline bool isNumeric(const uint64_t type)
{
    return (type == TypeOf<Smallint>().type()) ||
           (type == TypeOf<Int>().type()) ||
           (type == TypeOf<Bigint>().type()) ||
           (type == TypeOf<Real>().type()) ||
           (type == TypeOf<Float8>().type());
}

template <typename T>
inline constexpr bool isText()
{
    return std::is_same_v<T, Text>;
}

inline bool isText(const uint64_t type)
{
    return (type == TypeOf<Text>().type());
}

template <typename T>
inline constexpr bool isDate()
{
    return std::is_same_v<T, Date> ||
           std::is_same_v<T, Interval> ||
           std::is_same_v<T, Time> ||
           std::is_same_v<T, Timestamp>;
}

inline constexpr bool isDate(const uint64_t type)
{
    return (type == TypeOf<Date>().type()) ||
           (type == TypeOf<Interval>().type()) ||
           (type == TypeOf<Time>().type()) ||
           (type == TypeOf<Timestamp>().type());
}

}  // namespace esql::utils
