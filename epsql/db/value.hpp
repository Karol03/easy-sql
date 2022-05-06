/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <array>
#include <string>
#include <optional>

#include "types.hpp"


namespace epsql::db
{

struct Value
{
    virtual ~Value() = default;
    virtual void setNull() = 0;
    virtual operator bool() const = 0;
    virtual bool exists() const = 0;
};


template <typename T>
struct ValueT : public Value
{
    virtual T& operator*() = 0;
    virtual const T& operator*() const = 0;
    virtual T* operator->() = 0;
    virtual const T* operator->() const = 0;

    virtual const T* ptr() const = 0;
    virtual T* ptr() = 0;
    virtual T* nonEmptyPtr() = 0;
};


template <typename T>
struct NotNull : public ValueT<T>
{
public:
    using Type = NotNull<T>;
    using ValueType = T;

public:
    NotNull() = default;
    NotNull(const T& pvalue) : value{pvalue} {}
    NotNull(T&& pvalue) : value{std::move(pvalue)} {}
    NotNull(const NotNull&) = default;
    NotNull& operator=(const NotNull&) = default;
    NotNull(NotNull&&) noexcept = default;
    NotNull& operator=(NotNull&&) noexcept = default;
    NotNull& operator=(T& pvalue) { value = pvalue; return *this; }
    NotNull& operator=(T&& pvalue) noexcept { value = std::move(pvalue); return *this; }

    virtual inline T& operator*() override { return value; }
    virtual inline const T& operator*() const override { return value; }
    virtual inline T* operator->() override { return &value; }
    virtual inline const T* operator->() const override { return &value; }

    virtual inline const T* ptr() const override { return &value; }
    virtual inline T* ptr() override { return &value; }
    virtual inline T* nonEmptyPtr() override { return &value; }

    virtual inline operator bool() const override { return true; }
    virtual inline bool exists() const override { return true; }
    virtual inline void setNull() override { }

    inline static constexpr const char* name() { return "NOT NULL"; }
    inline static constexpr const char* referenceTable() { return ""; }
    inline static constexpr const char* referenceField() { return ""; }

protected:
    T value{};
};


template <typename T>
struct Nullable : public ValueT<T>
{
public:
    using Type = Nullable<T>;
    using ValueType = T;

public:
    Nullable() = default;
    Nullable(const T& pvalue) : value{pvalue} {}
    Nullable(T&& pvalue) : value{std::move(pvalue)} {}
    Nullable(const NotNull<T>& pvalue) : value{*pvalue} {}
    Nullable(NotNull<T>&& pvalue) : value{std::move(*pvalue)} {}
    Nullable(const Nullable&) = default;
    Nullable(Nullable&&) noexcept = default;

    Nullable& operator=(const Nullable&) = default;
    Nullable& operator=(Nullable&&) noexcept = default;
    Nullable& operator=(const T& pvalue) { value = pvalue; return *this; }
    Nullable& operator=(T&& pvalue) noexcept { value = std::move(pvalue); return *this; }
    Nullable& operator=(const NotNull<T>& pvalue) { value = *pvalue; return *this; }
    Nullable& operator=(NotNull<T>&& pvalue) { value = std::move(*pvalue); return *this; }

    virtual inline T& operator*() override { return *value; }
    virtual inline const T& operator*() const override { return *value; }
    virtual inline T* operator->() override { return &(*value); }
    virtual inline const T* operator->() const override { return &(*value); }

    virtual inline const T* ptr() const override { return value ? &(*value) : nullptr; }
    virtual inline T* ptr() override { return value ? &(*value) : nullptr; }
    virtual inline T* nonEmptyPtr() override { if (!value) value = T{}; return &(*value); }

    virtual inline operator bool() const override { return value.has_value(); }
    virtual inline bool exists() const override { return value.has_value(); }
    virtual inline void setNull() override { value = std::nullopt; }

    inline static constexpr const char* name() { return "NULL"; }
    inline static constexpr const char* referenceTable() { return ""; }
    inline static constexpr const char* referenceField() { return ""; }

protected:
    std::optional<T> value{std::nullopt};
};


struct IForeignKey
{
    virtual ~IForeignKey() = default;
};

template <typename NullType, int Reference>
struct ForeignKey : public IForeignKey, public NullType
{
    ForeignKey() = default;
    ForeignKey(const ForeignKey&) = default;
    ForeignKey(ForeignKey&&) noexcept = default;
    ForeignKey& operator=(const ForeignKey&) = default;
    ForeignKey& operator=(ForeignKey&&) noexcept = default;

    ForeignKey(const NullType& value) : NullType{value} {}
    ForeignKey(NullType&& value) : NullType{std::move(value)} {}
    ForeignKey& operator=(const NullType& value) { NullType::operator=(value); return *this; }
    ForeignKey& operator=(NullType&& value) noexcept { NullType::operator=(std::move(value)); return *this; }

    inline static constexpr const char* referenceTable() { return "Table"; }
    inline static constexpr const char* referenceField() { return "Field"; }
};


template <bool isNullable>
struct ForeignKeyDeducer {};

template <>
struct ForeignKeyDeducer<true>
{
    template <typename T, int Reference> using Type = ForeignKey<NotNull<T>, Reference>;
};

template <>
struct ForeignKeyDeducer<false>
{
    template <typename T, int Reference> using Type = ForeignKey<Nullable<T>, Reference>;
};

template <int V>
inline constexpr bool is_positive_v = (V > 0);

template <int fieldUuid>
struct ForeignKeyDeduce
{
    template <typename T>
    using Type = typename ForeignKeyDeducer<is_positive_v<fieldUuid>>::Type<T, fieldUuid>;

private:
    static constexpr bool is_positive() { return fieldUuid > 0; }
};


}  // namespace epsql::db
