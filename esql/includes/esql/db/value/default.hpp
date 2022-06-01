/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <string>

#include "value.hpp"


namespace esql::db::value
{

template <typename NullType>
struct Default : public NullType
{
    Default() = default;

    template <typename T>
    Default(const Default<T>& def) noexcept
    {
        if (def.exists())
            NullType::value = *def;
    }

    template <typename T>
    Default(Default<T>&& def) noexcept
    {
        if (def.exists())
            NullType::value = *def;
        def.setNull();
    }

    template <typename T>
    Default& operator=(const Default<T>& def) noexcept
    {
        if (def.exists())
            NullType::value = *def;
        return *this;
    }

    template <typename T>
    Default& operator=(Default<T>&& def) noexcept
    {
        if (def.exists())
            NullType::value = *def;
        def.setNull();
        return *this;
    }

    Default(const NullType& value) : NullType{value} {}
    Default(NullType&& value) : NullType{std::move(value)} {}
    Default& operator=(const NullType& value) { NullType::operator=(value); return *this; }
    Default& operator=(NullType&& value) noexcept { NullType::operator=(std::move(value)); return *this; }
};


template <int isNullable>
struct DefaultDeducer {};

template <>
struct DefaultDeducer<1>
{
    template <typename T> using Type = Default<NotNull<T>>;
};

template <>
struct DefaultDeducer<-1>
{
    template <typename T> using Type = Default<Nullable<T>>;
};

template <int nullability>
struct DefaultDeduce
{
    template <typename T>
    using Type = typename DefaultDeducer<nullability>::template Type<T>;
};

}  // namespace esql::db::value
