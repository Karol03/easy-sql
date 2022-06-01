/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <optional>

#include "ivalue.hpp"


namespace esql::db::value
{

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

    virtual inline operator bool() const noexcept override { return true; }
    virtual inline bool exists() const noexcept override { return true; }
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

    virtual inline operator bool() const noexcept override { return value.has_value(); }
    virtual inline bool exists() const noexcept override { return value.has_value(); }
    virtual inline void setNull() override { value = std::nullopt; }

    inline static constexpr const char* name() { return "NULL"; }
    inline static constexpr const char* referenceTable() { return ""; }
    inline static constexpr const char* referenceField() { return ""; }

protected:
    std::optional<T> value{std::nullopt};
};


template <typename T>
inline bool compare(const Nullable<T>& lhs, const NotNull<T>& rhs)
{
    return lhs.exists() && (*lhs == *rhs);
}

template <typename T>
inline bool compare(const NotNull<T>& lhs, const Nullable<T>& rhs)
{
    return rhs.exists() && (*lhs == *rhs);
}

template <typename T>
inline bool compare(const NotNull<T>& lhs, const NotNull<T>& rhs)
{
    return *lhs == *rhs;
}

template <typename T>
inline bool compare(const Nullable<T>& lhs, const Nullable<T>& rhs)
{
    return (!lhs && !rhs) || (lhs && rhs && (*lhs == *rhs));
}

template <typename T>
inline bool compare(const NotNull<T>& lhs, const T& rhs)
{
    return *lhs == rhs;
}

template <typename T>
inline bool compare(const Nullable<T>& lhs, const T& rhs)
{
    return lhs && (*lhs == rhs);
}

}  // namespace esql::db::value
