/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <cmath>
#include <inttypes.h>
#include <type_traits>

#include "esql/db/table/reflectiongroup.hpp"
#include "value.hpp"


namespace esql::db::value
{

template <typename NullType, int64_t Reference>
struct ForeignKey : public NullType
{
    ForeignKey() = default;

    template <typename T, int64_t Ref,
              typename = std::enable_if_t<std::is_same_v<typename NullType::ValueType, ForeignKey<T, Ref>::ValueType>>>
    ForeignKey(const ForeignKey<T, Ref>& fk) noexcept
    {
        if (fk.exists())
            NullType::value = *fk;
    }

    template <typename T, int64_t Ref,
              typename = std::enable_if_t<std::is_same_v<typename NullType::ValueType, ForeignKey<T, Ref>::ValueType>>>
    ForeignKey(ForeignKey<T, Ref>&& fk) noexcept
    {
        if (fk.exists())
            NullType::value = *fk;
        fk.setNull();
    }

    template <typename T, int64_t Ref,
              typename = std::enable_if_t<std::is_same_v<typename NullType::ValueType, ForeignKey<T, Ref>::ValueType>>>
    ForeignKey& operator=(const ForeignKey<T, Ref>& fk) noexcept
    {
        if (fk.exists())
            NullType::value = *fk;
        return *this;
    }

    template <typename T, int64_t Ref,
              typename = std::enable_if_t<std::is_same_v<typename NullType::ValueType, ForeignKey<T, Ref>::ValueType>>>
    ForeignKey& operator=(ForeignKey<T, Ref>&& fk) noexcept
    {
        if (fk.exists())
            NullType::value = *fk;
        fk.setNull();
        return *this;
    }

    ForeignKey(const NullType& value) : NullType{value} {}
    ForeignKey(NullType&& value) : NullType{std::move(value)} {}
    ForeignKey& operator=(const NullType& value) { NullType::operator=(value); return *this; }
    ForeignKey& operator=(NullType&& value) noexcept { NullType::operator=(std::move(value)); return *this; }

    inline static const char* referenceTable()
    {
        static std::string referenceTableName = cutTableName();
        return referenceTableName.c_str();
    }

    inline static const char* referenceField()
    {
        static std::string referenceFieldName = cutFieldName();
        return referenceFieldName.c_str();
    }

private:
    inline static std::string cutTableName()
    {
        const auto fullName = db::table::ReflectionRegister::reference(std::abs(Reference));
        if (!fullName)
            return {};

        const auto it = fullName->find(':');
        if (it == std::string::npos)
            return {};

        return fullName->substr(0, it);
    }

    inline static std::string cutFieldName()
    {
        const auto fullName = db::table::ReflectionRegister::reference(std::abs(Reference));
        if (!fullName)
            return {};

        const auto it = fullName->find(':');
        if (it == std::string::npos || (it + 2) >= fullName->size())
            return {};

        return fullName->substr(it + 2);
    }
};


template <bool isNullable>
struct ForeignKeyDeducer {};

template <>
struct ForeignKeyDeducer<true>
{
    template <typename T, int64_t Reference> using Type = ForeignKey<NotNull<T>, Reference>;
};

template <>
struct ForeignKeyDeducer<false>
{
    template <typename T, int64_t Reference> using Type = ForeignKey<Nullable<T>, Reference>;
};

template <int64_t V>
inline constexpr bool is_positive_v = (V > 0);

template <int64_t fieldUuid>
struct ForeignKeyDeduce
{
    template <typename T>
    using Type = typename ForeignKeyDeducer<is_positive_v<fieldUuid>>::template Type<T, fieldUuid>;
};

}  // namespace esql::db::value
