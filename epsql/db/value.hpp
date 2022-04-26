/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <optional>


namespace epsql::db
{

struct Value
{
    virtual ~Value() = default;
    virtual void setNull() = 0;
};


template <typename T>
struct Nullable : public Value
{
public:
    Nullable() = default;
    Nullable(const T& pvalue) : value{pvalue} {}
    Nullable(T&& pvalue) : value{std::move(pvalue)} {}
    Nullable(const Nullable&) = default;
    Nullable& operator=(const Nullable&) = default;
    Nullable(Nullable&&) noexcept = default;
    Nullable& operator=(Nullable&&) noexcept = default;

    inline T& operator*() { return *value; }
    inline const T& operator*() const { return *value; }
    inline T* operator->() { return &(*value); }
    inline const T* operator->() const { return &(*value); }

    inline operator bool() const { return value.has_value(); }
    inline T* nonEmptyPtr()
    {
        if (!value)
            value = T{};
        return &(*value);
    }
    virtual void setNull() override { value = std::nullopt; }

public:
    std::optional<T> value{std::nullopt};
};


template <typename T>
struct NotNull : public Value
{
public:
    NotNull() = default;
    NotNull(const T& pvalue) : value{pvalue} {}
    NotNull(T&& pvalue) : value{std::move(pvalue)} {}
    NotNull(const NotNull&) = default;
    NotNull& operator=(const NotNull&) = default;
    NotNull(NotNull&&) noexcept = default;
    NotNull& operator=(NotNull&&) noexcept = default;

    inline T& operator*() { return value; }
    inline const T& operator*() const { return value; }
    inline T* operator->() { return &value; }
    inline const T* operator->() const { return &value; }

    inline operator bool() const { return true; }
    inline T* nonEmptyPtr() { return &value; }
    virtual void setNull() override { }

public:
    T value{};
};

}  // namespace epsql::db
