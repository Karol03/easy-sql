/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>

#include "esql/utils/typeof.hpp"
#include "fieldmanager.hpp"
#include "semantic/operation.hpp"
#include "semantic/operationresult.hpp"


namespace esql::db::lambda
{

struct Field
{
public:
    Field(FieldManager& manager,
          const char* tableAndFieldName,
          uint64_t fieldTypeId,
          const char* nullableName);

    template <typename T> friend FieldManager::OperationResult operator==(const T& lhs, const Field& rhs);
    template <typename T> friend FieldManager::OperationResult operator==(const Field& lhs, const T& rhs);

    template <typename T> friend FieldManager::OperationResult operator!=(const T& lhs, const Field& rhs);
    template <typename T> friend FieldManager::OperationResult operator!=(const Field& lhs, const T& rhs);

    template <typename T> friend FieldManager::OperationResult operator>(const T& lhs, const Field& rhs);
    template <typename T> friend FieldManager::OperationResult operator>(const Field& lhs, const T& rhs);

    template <typename T> friend FieldManager::OperationResult operator>=(const T& lhs, const Field& rhs);
    template <typename T> friend FieldManager::OperationResult operator>=(const Field& lhs, const T& rhs);

    template <typename T> friend FieldManager::OperationResult operator<(const T& lhs, const Field& rhs);
    template <typename T> friend FieldManager::OperationResult operator<(const Field& lhs, const T& rhs);

    template <typename T> friend FieldManager::OperationResult operator<=(const T& lhs, const Field& rhs);
    template <typename T> friend FieldManager::OperationResult operator<=(const Field& lhs, const T& rhs);

    friend FieldManager::OperationResult operator==(const Field& lhs, const Field& rhs);
    friend FieldManager::OperationResult operator!=(const Field& lhs, const Field& rhs);
    friend FieldManager::OperationResult operator>(const Field& lhs, const Field& rhs);
    friend FieldManager::OperationResult operator>=(const Field& lhs, const Field& rhs);
    friend FieldManager::OperationResult operator<(const Field& lhs, const Field& rhs);
    friend FieldManager::OperationResult operator<=(const Field& lhs, const Field& rhs);

    template <typename T>
    std::enable_if_t<utils::isNumeric<T>,
    FieldManager::OperationResult> isBetween(T min, T max)
    {
        if (!utils::isNumeric(m_fieldTypeId))
        {
            auto result = std::stringstream{};
            result << "Cannot use isBetween(" << min << ", " << max
                   << ") on '" << m_tableFieldName << "', field is not numeric type!";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationBeetween>(m_tableFieldName, table::getValue(min), table::getValue(max));
    }

    template <typename T>
    std::enable_if_t<utils::isDate<T>,
    FieldManager::OperationResult> isBetween(T min, T max)
    {
        if (!utils::isDate(m_fieldTypeId))
        {
            auto result = std::stringstream{};
            result << "Cannot use isBetween(" << min << ", " << max
                   << ") on '" << m_tableFieldName << "', field is not date type!";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationBeetween>(m_tableFieldName, table::getValue(min), table::getValue(max));
    }

    template <typename T>
    std::enable_if_t<utils::isNumeric<T>,
    FieldManager::OperationResult> isNotBetween(T min, T max)
    {
        if (!utils::isNumeric(m_fieldTypeId))
        {
            auto result = std::stringstream{};
            result << "Cannot use isNotBetween(" << min << ", " << max
                   << ") on '" << m_tableFieldName << "', field is not numeric type!";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationNotBeetween>(m_tableFieldName, table::getValue(min), table::getValue(max));
    }

    template <typename T>
    std::enable_if_t<utils::isDate<T>,
    FieldManager::OperationResult> isNotBetween(T min, T max)
    {
        if (!utils::isDate(m_fieldTypeId))
        {
            auto result = std::stringstream{};
            result << "Cannot use isNotBetween(" << min << ", " << max
                   << ") on '" << m_tableFieldName << "', field is not date type!";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationNotBeetween>(m_tableFieldName, table::getValue(min), table::getValue(max));
    }

    template <typename T>
    FieldManager::OperationResult isInRange(std::vector<T> range)
    {
        if (range.empty())
        {
            auto result = std::stringstream{};
            result << "Cannot use isInRange(...) on '" << m_tableFieldName << "' with empty range!";
            throw std::runtime_error{result.str()};
        }

        auto result = std::vector<std::string>{};
        result.reserve(range.size());
        for (const auto& value : range)
            result.push_back(table::getValue(value));
        return m_manager.operation<semantic::OperationAnyOf>(m_tableFieldName, std::move(result));
    }

    template <typename T>
    FieldManager::OperationResult isNotInRange(std::vector<T> range)
    {
        if (range.empty())
        {
            auto result = std::stringstream{};
            result << "Cannot use isNotInRange(...) on '" << m_tableFieldName << "' with empty range!";
            throw std::runtime_error{result.str()};
        }

        auto result = std::vector<std::string>{};
        result.reserve(range.size());
        for (const auto& value : range)
            result.push_back(table::getValue(value));
        return m_manager.operation<semantic::OperationNoneOf>(m_tableFieldName, std::move(result));
    }

    inline FieldManager::OperationResult isNull()
    {
        if (!m_isNullable)
        {
            auto result = std::stringstream{};
            result << "Cannot use isNull(...) on '" << m_tableFieldName << "', field is not NULLABLE and result is always 'false'!";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationIsNull>(m_tableFieldName);
    }

    inline FieldManager::OperationResult isNotNull()
    {
        if (!m_isNullable)
        {
            auto result = std::stringstream{};
            result << "Cannot use isNotNull(...) on '" << m_tableFieldName << "', field is not NULLABLE and result is always 'true'";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationIsNotNull>(m_tableFieldName);
    }

    inline FieldManager::OperationResult startsWith(std::string prefix)
    {
        if (!utils::isText(m_fieldTypeId))
        {
            auto result = std::stringstream{};
            result << "Cannot use startsWith('" << prefix << "') on '"
                   << m_tableFieldName << "', field is not Text type!";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationStartsWith>(m_tableFieldName, std::move(prefix));
    }

    inline FieldManager::OperationResult endsWith(std::string postfix)
    {
        if (!utils::isText(m_fieldTypeId))
        {
            auto result = std::stringstream{};
            result << "Cannot use endsWith('" << postfix << "') on '"
                   << m_tableFieldName << "', field is not Text type!";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationEndsWith>(m_tableFieldName, std::move(postfix));
    }

    inline FieldManager::OperationResult match(std::string regexp)
    {
        if (!utils::isText(m_fieldTypeId))
        {
            auto result = std::stringstream{};
            result << "Cannot use match('" << regexp << "') on '"
                   << m_tableFieldName << "', field is not Text type!";
            throw std::runtime_error{result.str()};
        }
        return m_manager.operation<semantic::OperationRegexp>(m_tableFieldName, std::move(regexp));
    }

private:
    bool m_isNullable;
    FieldManager& m_manager;
    uint64_t m_fieldTypeId;
    std::string m_tableFieldName;
};


template <typename T>
inline FieldManager::OperationResult operator==(const T& lhs, const Field& rhs)
{
    return operator==(rhs, lhs);
}
template <typename T>
inline FieldManager::OperationResult operator==(const Field& lhs, const T& rhs)
{
    return lhs.m_manager.operation<semantic::OperationEqual>(lhs.m_tableFieldName, table::getValue(rhs));
}

template <typename T>
inline FieldManager::OperationResult operator!=(const T& lhs, const Field& rhs)
{
    return operator!=(rhs, lhs);
}
template <typename T>
inline FieldManager::OperationResult operator!=(const Field& lhs, const T& rhs)
{
    return lhs.m_manager.operation<semantic::OperationDifferentThan>(lhs.m_tableFieldName, table::getValue(rhs));
}

template <typename T>
inline FieldManager::OperationResult operator>(const T& lhs, const Field& rhs)
{
    return operator<(rhs, lhs);
}
template <typename T>
inline FieldManager::OperationResult operator>(const Field& lhs, const T& rhs)
{
    return lhs.m_manager.operation<semantic::OperationBigger>(lhs.m_tableFieldName, table::getValue(rhs));
}

template <typename T>
inline FieldManager::OperationResult operator>=(const T& lhs, const Field& rhs)
{
    return operator<=(rhs, lhs);
}
template <typename T>
inline FieldManager::OperationResult operator>=(const Field& lhs, const T& rhs)
{
    return lhs.m_manager.operation<semantic::OperationBiggerEqual>(lhs.m_tableFieldName, table::getValue(rhs));
}

template <typename T>
inline FieldManager::OperationResult operator<(const T& lhs, const Field& rhs)
{
    return operator>(rhs, lhs);
}
template <typename T>
inline FieldManager::OperationResult operator<(const Field& lhs, const T& rhs)
{
    return lhs.m_manager.operation<semantic::OperationLess>(lhs.m_tableFieldName, table::getValue(rhs));
}

template <typename T>
inline FieldManager::OperationResult operator<=(const T& lhs, const Field& rhs)
{
    return operator>=(rhs, lhs);
}
template <typename T>
inline FieldManager::OperationResult operator<=(const Field& lhs, const T& rhs)
{
    return lhs.m_manager.operation<semantic::OperationLessEqual>(lhs.m_tableFieldName, table::getValue(rhs));
}

inline FieldManager::OperationResult operator==(const Field& lhs, const Field& rhs)
{
    return lhs.m_manager.operation<semantic::OperationFieldEqual>(lhs.m_tableFieldName, rhs.m_tableFieldName);
}

inline FieldManager::OperationResult operator!=(const Field& lhs, const Field& rhs)
{
    return lhs.m_manager.operation<semantic::OperationFieldDifferent>(lhs.m_tableFieldName, rhs.m_tableFieldName);
}

inline FieldManager::OperationResult operator>(const Field& lhs, const Field& rhs)
{
    return lhs.m_manager.operation<semantic::OperationFieldBigger>(lhs.m_tableFieldName, rhs.m_tableFieldName);
}

inline FieldManager::OperationResult operator>=(const Field& lhs, const Field& rhs)
{
    return lhs.m_manager.operation<semantic::OperationFieldBiggerEqual>(lhs.m_tableFieldName, rhs.m_tableFieldName);
}

inline FieldManager::OperationResult operator<(const Field& lhs, const Field& rhs)
{
    return lhs.m_manager.operation<semantic::OperationFieldLess>(lhs.m_tableFieldName, rhs.m_tableFieldName);
}

inline FieldManager::OperationResult operator<=(const Field& lhs, const Field& rhs)
{
    return lhs.m_manager.operation<semantic::OperationFieldLessEqual>(lhs.m_tableFieldName, rhs.m_tableFieldName);
}

}  // namespace esql::db::lambda
