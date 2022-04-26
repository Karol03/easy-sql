/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "operation.hpp"


namespace epsql::db::lambda
{

struct __FieldManager
{
private:


public:
    inline std::string result()
    {
        return std::string{};
    }

    inline std::size_t operator[](const char* name)
    {
        auto& id = m_keys[name];
        if (id == 0)
            id = nextField();
        return id;
    }

    template <typename T, typename... Args>
    inline __OperationResult operation(std::size_t id, Args&&... args)
    {
        ++m_counts[id];
        m_operations.push_back(make<T>(id, std::forward<Args>(args)...));
        return {};
    }

private:
    inline std::size_t nextField() { return ++m_field; }

private:
    std::unordered_map<std::string, std::size_t> m_keys;
    std::unordered_map<std::size_t, std::size_t> m_counts;
    std::vector<std::unique_ptr<__Operation>> m_operations;
    std::size_t m_field{};
};


struct __Field
{
public:
    __Field(__FieldManager& manager, const char* name)
        : m_manager{manager}
    {
        m_id = m_manager[name];
    }

    template <typename T> friend __OperationResult operator==(const T& lhs, const __Field& rhs);
    template <typename T> friend __OperationResult operator==(const __Field& lhs, const T& rhs);

    template <typename T> friend __OperationResult operator!=(const T& lhs, const __Field& rhs);
    template <typename T> friend __OperationResult operator!=(const __Field& lhs, const T& rhs);

    template <typename T> friend __OperationResult operator>(const T& lhs, const __Field& rhs);
    template <typename T> friend __OperationResult operator>(const __Field& lhs, const T& rhs);

    template <typename T> friend __OperationResult operator>=(const T& lhs, const __Field& rhs);
    template <typename T> friend __OperationResult operator>=(const __Field& lhs, const T& rhs);

    template <typename T> friend __OperationResult operator<(const T& lhs, const __Field& rhs);
    template <typename T> friend __OperationResult operator<(const __Field& lhs, const T& rhs);

    template <typename T> friend __OperationResult operator<=(const T& lhs, const __Field& rhs);
    template <typename T> friend __OperationResult operator<=(const __Field& lhs, const T& rhs);

    friend __OperationResult operator==(const __Field& lhs, const __Field& rhs);
    friend __OperationResult operator!=(const __Field& lhs, const __Field& rhs);
    friend __OperationResult operator>(const __Field& lhs, const __Field& rhs);
    friend __OperationResult operator>=(const __Field& lhs, const __Field& rhs);
    friend __OperationResult operator<(const __Field& lhs, const __Field& rhs);
    friend __OperationResult operator<=(const __Field& lhs, const __Field& rhs);

    template <typename T>
    std::enable_if_t<std::is_arithmetic_v<T>,
    __OperationResult> between(T min, T max) { return m_manager.operation<__OperationBeetween>(m_id, min, max); }

    template <typename T>
    std::enable_if_t<std::is_arithmetic_v<T>,
    __OperationResult> inRange(std::vector<T> range) { return m_manager.operation<__OperationRange>(m_id, std::move(range)); }

    inline __OperationResult isNotNull() { return m_manager.operation<__OperationIsNotNull>(m_id); }
    inline __OperationResult isNull() { return m_manager.operation<__OperationIsNull>(m_id); }
    inline __OperationResult endsWith(std::string postfix) { return m_manager.operation<__OperationEndsWith>(m_id, std::move(postfix)); }
    inline __OperationResult startsWith(std::string prefix) { return m_manager.operation<__OperationStartsWith>(m_id, std::move(prefix)); }
    inline __OperationResult match(std::string regexp) { return m_manager.operation<__OperationRegexp>(m_id, std::move(regexp)); }

private:
    __FieldManager& m_manager;
    std::size_t m_id;
};


template <typename T> inline __OperationResult operator==(const T& lhs, const __Field& rhs) { return operator==(rhs, lhs); }
template <typename T> inline __OperationResult operator==(const __Field& lhs, const T& rhs) { return lhs.m_manager.operation<__OperationEqual<T>>(lhs.m_id, rhs); }

template <typename T> inline __OperationResult operator!=(const T& lhs, const __Field& rhs) { return operator!=(rhs, lhs); }
template <typename T> inline __OperationResult operator!=(const __Field& lhs, const T& rhs) { return lhs.m_manager.operation<__OperationNotEqual<T>>(lhs.m_id, rhs); }

template <typename T> inline __OperationResult operator>(const T& lhs, const __Field& rhs) { return operator<(rhs, lhs); }
template <typename T> inline __OperationResult operator>(const __Field& lhs, const T& rhs) { return lhs.m_manager.operation<__OperationBigger<T>>(lhs.m_id, rhs); }

template <typename T> inline __OperationResult operator>=(const T& lhs, const __Field& rhs) { return operator<=(rhs, lhs); }
template <typename T> inline __OperationResult operator>=(const __Field& lhs, const T& rhs) { return lhs.m_manager.operation<__OperationBiggerEqual<T>>(lhs.m_id, rhs); }

template <typename T> inline __OperationResult operator<(const T& lhs, const __Field& rhs) { return operator>(rhs, lhs); }
template <typename T> inline __OperationResult operator<(const __Field& lhs, const T& rhs) { return lhs.m_manager.operation<__OperationLess<T>>(lhs.m_id, rhs); }

template <typename T> inline __OperationResult operator<=(const T& lhs, const __Field& rhs) { return operator>=(rhs, lhs); }
template <typename T> inline __OperationResult operator<=(const __Field& lhs, const T& rhs) { return lhs.m_manager.operation<__OperationLessEqual<T>>(lhs.m_id, rhs); }

inline __OperationResult operator==(const __Field& lhs, const __Field& rhs) { return lhs.m_manager.operation<__OperationFieldEqual>(lhs.m_id, rhs.m_id); }
inline __OperationResult operator!=(const __Field& lhs, const __Field& rhs) { return lhs.m_manager.operation<__OperationFieldNotEqual>(lhs.m_id, rhs.m_id); }
inline __OperationResult operator>(const __Field& lhs, const __Field& rhs)  { return lhs.m_manager.operation<__OperationFieldLess>(lhs.m_id, rhs.m_id); }
inline __OperationResult operator>=(const __Field& lhs, const __Field& rhs) { return lhs.m_manager.operation<__OperationFieldLessEqual>(lhs.m_id, rhs.m_id); }
inline __OperationResult operator<(const __Field& lhs, const __Field& rhs)  { return lhs.m_manager.operation<__OperationFieldBigger>(lhs.m_id, rhs.m_id); }
inline __OperationResult operator<=(const __Field& lhs, const __Field& rhs) { return lhs.m_manager.operation<__OperationFieldBiggerEqual>(lhs.m_id, rhs.m_id); }

}  // namespace epsql::db::lambda
