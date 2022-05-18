/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once


namespace epsql::db::value
{

struct Value
{
    virtual ~Value() = default;
    virtual void setNull() = 0;
    virtual operator bool() const noexcept = 0;
    virtual bool exists() const noexcept = 0;
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

}  // namespace epsql::db::value
