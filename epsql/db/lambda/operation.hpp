/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <memory>
#include <string>
#include <vector>


namespace epsql::db::lambda
{

class __Operation
{
public:
    __Operation(std::size_t fieldId) : m_fieldId{fieldId} {}
    virtual ~__Operation() = default;

    inline std::size_t field() const { return m_fieldId; }

private:
    std::size_t m_fieldId;
};


template <typename T>
class __OperationBeetween : public __Operation
{
public:
    __OperationBeetween(std::size_t id, T min, T max)
        : __Operation{id}
        , min{min}
        , max{max}
    {}

public:
    T min, max;
};


template <typename T>
class __OperationRange : public __Operation
{
public:
    __OperationRange(std::size_t id, std::vector<T> range)
        : __Operation(id)
        , range{std::move(range)}
    {}

public:
    std::vector<T> range;
};


class __OperationIsNull : public __Operation
{
public:
    __OperationIsNull(std::size_t id)
        : __Operation{id}
    {}
};


class __OperationIsNotNull : public __Operation
{
public:
    __OperationIsNotNull(std::size_t id)
        : __Operation{id}
    {}
};


class __OperationStartsWith : public __Operation
{
public:
    __OperationStartsWith(std::size_t id, std::string prefix)
        : __Operation{id}
        , prefix{std::move(prefix)}
    {}

public:
    std::string prefix;
};


class __OperationEndsWith : public __Operation
{
public:
    __OperationEndsWith(std::size_t id, std::string postfix)
        : __Operation{id}
        , postfix{std::move(postfix)}
    {}

public:
    std::string postfix;
};


class __OperationRegexp : public __Operation
{
public:
    __OperationRegexp(std::size_t id, std::string regexp)
        : __Operation{id}
        , regexp{std::move(regexp)}
    {}

public:
    std::string regexp;
};


template <typename T>
class __OperationEqual : public __Operation
{
public:
    __OperationEqual(std::size_t id, T value)
        : __Operation{id}
        , value{std::move(value)}
    {}

public:
    T value;
};


template <typename T>
class __OperationNotEqual : public __Operation
{
public:
    __OperationNotEqual(std::size_t id, T value)
        : __Operation{id}
        , value{std::move(value)}
    {}

public:
    T value;
};


template <typename T>
class __OperationLess : public __Operation
{
public:
    __OperationLess(std::size_t id, T value)
        : __Operation{id}
        , value{std::move(value)}
    {}

public:
    T value;
};


template <typename T>
class __OperationLessEqual : public __Operation
{
public:
    __OperationLessEqual(std::size_t id, T value)
        : __Operation{id}
        , value{std::move(value)}
    {}

public:
    T value;
};


template <typename T>
class __OperationBigger : public __Operation
{
public:
    __OperationBigger(std::size_t id, T value)
        : __Operation{id}
        , value{std::move(value)}
    {}

public:
    T value;
};


template <typename T>
class __OperationBiggerEqual : public __Operation
{
public:
    __OperationBiggerEqual(std::size_t id, T value)
        : __Operation{id}
        , value{std::move(value)}
    {}

public:
    T value;
};


class __OperationFieldEqual : public __Operation
{
public:
    __OperationFieldEqual(std::size_t id, std::size_t secondId)
        : __Operation{id}
        , secondId{secondId}
    {}

public:
    std::size_t secondId;
};


class __OperationFieldNotEqual : public __Operation
{
public:
    __OperationFieldNotEqual(std::size_t id, std::size_t secondId)
        : __Operation{id}
        , secondId{secondId}
    {}

public:
    std::size_t secondId;
};


class __OperationFieldLess : public __Operation
{
public:
    __OperationFieldLess(std::size_t id, std::size_t secondId)
        : __Operation{id}
        , secondId{secondId}
    {}

public:
    std::size_t secondId;
};


class __OperationFieldLessEqual : public __Operation
{
public:
    __OperationFieldLessEqual(std::size_t id, std::size_t secondId)
        : __Operation{id}
        , secondId{secondId}
    {}

public:
    std::size_t secondId;
};


class __OperationFieldBigger : public __Operation
{
public:
    __OperationFieldBigger(std::size_t id, std::size_t secondId)
        : __Operation{id}
        , secondId{secondId}
    {}

public:
    std::size_t secondId;
};


class __OperationFieldBiggerEqual : public __Operation
{
public:
    __OperationFieldBiggerEqual(std::size_t id, std::size_t secondId)
        : __Operation{id}
        , secondId{secondId}
    {}

public:
    std::size_t secondId;
};


class __OperationResult
{
public:
    __OperationResult()
    {
        if (!m_isNewOperationPossible)
            throw std::runtime_error{"Not possible "};
    }

    operator bool() const { return true; }

private:
    bool m_isNewOperationPossible{true};
};


template <typename T, typename... Args>
inline auto make(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

}  // namespace epsql::db::lambda
