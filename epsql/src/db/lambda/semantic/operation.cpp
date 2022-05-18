/**
 * Created by Karol Dudzic @ 2022
 */

#include "epsql/db/lambda/semantic/operation.hpp"

#include <sstream>


namespace epsql::db::lambda::semantic
{

// ----------------------------------------------------

OperationBeetween::OperationBeetween(std::string tableAndFieldName, std::string min, std::string max)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_min{std::move(min)}
    , m_max{std::move(max)}
{}

std::string OperationBeetween::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " BEETWEEN " << m_min << " AND " << m_max << ")";
    return result.str();
}

std::vector<std::string> OperationBeetween::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------


OperationNotBeetween::OperationNotBeetween(std::string tableAndFieldName, std::string min, std::string max)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_min{std::move(min)}
    , m_max{std::move(max)}
{}

std::string OperationNotBeetween::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " NOT BEETWEEN " << m_min << " AND " << m_max << ")";
    return result.str();
}

std::vector<std::string> OperationNotBeetween::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationAnyOf::OperationAnyOf(std::string tableAndFieldName, std::vector<std::string> range)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_range{std::move(range)}
{}

std::string OperationAnyOf::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " = ANY (ARRAY[" << m_range.front();

    for (auto i = 1ul; i < m_range.size(); ++i)
        result << ", " << m_range[i];

    result << "]))";
    return result.str();
}

std::vector<std::string> OperationAnyOf::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationNoneOf::OperationNoneOf(std::string tableAndFieldName, std::vector<std::string> range)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_range{std::move(range)}
{}

std::string OperationNoneOf::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " <> ALL (ARRAY[" << m_range.front();

    for (auto i = 1ul; i < m_range.size(); ++i)
        result << ", " << m_range[i];

    result << "]))";
    return result.str();
}

std::vector<std::string> OperationNoneOf::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------


OperationIsNull::OperationIsNull(std::string tableAndFieldName)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
{}

std::string OperationIsNull::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " IS NULL)";
    return result.str();
}

std::vector<std::string> OperationIsNull::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationIsNotNull::OperationIsNotNull(std::string tableAndFieldName)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
{}

std::string OperationIsNotNull::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " IS NOT NULL)";
    return result.str();
}

std::vector<std::string> OperationIsNotNull::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationStartsWith::OperationStartsWith(std::string tableAndFieldName, std::string prefix)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_prefix{std::move(prefix)}
{}

std::string OperationStartsWith::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " = '" << m_prefix << "%')";
    return result.str();
}

std::vector<std::string> OperationStartsWith::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationEndsWith::OperationEndsWith(std::string tableAndFieldName, std::string postfix)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_postfix{std::move(postfix)}
{}

std::string OperationEndsWith::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " = '%" << m_postfix << "')";
    return result.str();
}

std::vector<std::string> OperationEndsWith::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationRegexp::OperationRegexp(std::string tableAndFieldName, std::string regexp)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_regexp{std::move(regexp)}
{}

std::string OperationRegexp::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " = '" << m_regexp << "')";
    return result.str();
}

std::vector<std::string> OperationRegexp::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationEqual::OperationEqual(std::string tableAndFieldName, std::string value)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_value{std::move(value)}
{}

std::string OperationEqual::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " = " << m_value << ")";
    return result.str();
}

std::vector<std::string> OperationEqual::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationDifferentThan::OperationDifferentThan(std::string tableAndFieldName, std::string value)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_value{std::move(value)}
{}

std::string OperationDifferentThan::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " <> " << m_value << ")";
    return result.str();
}

std::vector<std::string> OperationDifferentThan::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationLess::OperationLess(std::string tableAndFieldName, std::string value)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_value{std::move(value)}
{}

std::string OperationLess::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " < " << m_value << ")";
    return result.str();
}

std::vector<std::string> OperationLess::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationLessEqual::OperationLessEqual(std::string tableAndFieldName, std::string value)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_value{std::move(value)}
{}

std::string OperationLessEqual::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " <= " << m_value << ")";
    return result.str();
}

std::vector<std::string> OperationLessEqual::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationBigger::OperationBigger(std::string tableAndFieldName, std::string value)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_value{std::move(value)}
{}

std::string OperationBigger::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " > " << m_value << ")";
    return result.str();
}

std::vector<std::string> OperationBigger::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationBiggerEqual::OperationBiggerEqual(std::string tableAndFieldName, std::string value)
    : m_tableAndFieldName{std::move(tableAndFieldName)}
    , m_value{std::move(value)}
{}

std::string OperationBiggerEqual::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_tableAndFieldName << " >= " << m_value << ")";
    return result.str();
}

std::vector<std::string> OperationBiggerEqual::tables() const
{
    return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationFieldEqual::OperationFieldEqual(std::string firstTableAndFieldName,
                                         std::string secondTableAndFieldName)
    : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
    , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
{}

std::string OperationFieldEqual::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_firstTableAndFieldName << " = "
           << m_secondTableAndFieldName << ")";
    return result.str();
}

std::vector<std::string> OperationFieldEqual::tables() const
{
    return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
            m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationFieldDifferent::OperationFieldDifferent(std::string firstTableAndFieldName,
                                                 std::string secondTableAndFieldName)
    : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
    , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
{}

std::string OperationFieldDifferent::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_firstTableAndFieldName << " <> "
           << m_secondTableAndFieldName << ")";
    return result.str();
}

std::vector<std::string> OperationFieldDifferent::tables() const
{
    return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
            m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationFieldBigger::OperationFieldBigger(std::string firstTableAndFieldName,
                                           std::string secondTableAndFieldName)
    : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
    , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
{}

std::string OperationFieldBigger::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_firstTableAndFieldName << " > "
           << m_secondTableAndFieldName << ")";
    return result.str();
}

std::vector<std::string> OperationFieldBigger::tables() const
{
    return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
            m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationFieldBiggerEqual::OperationFieldBiggerEqual(std::string firstTableAndFieldName,
                                                     std::string secondTableAndFieldName)
    : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
    , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
{}

std::string OperationFieldBiggerEqual::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_firstTableAndFieldName << " >= "
           << m_secondTableAndFieldName << ")";
    return result.str();
}

std::vector<std::string> OperationFieldBiggerEqual::tables() const
{
    return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
            m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationFieldLess::OperationFieldLess(std::string firstTableAndFieldName,
                                       std::string secondTableAndFieldName)
    : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
    , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
{}

std::string OperationFieldLess::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_firstTableAndFieldName << " < "
           << m_secondTableAndFieldName << ")";
    return result.str();
}

std::vector<std::string> OperationFieldLess::tables() const
{
    return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
            m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
}

// ----------------------------------------------------

OperationFieldLessEqual::OperationFieldLessEqual(std::string firstTableAndFieldName,
                                                 std::string secondTableAndFieldName)
    : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
    , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
{}

std::string OperationFieldLessEqual::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_firstTableAndFieldName << " <= "
           << m_secondTableAndFieldName << ")";
    return result.str();
}

std::vector<std::string> OperationFieldLessEqual::tables() const
{
    return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
            m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
}

// ----------------------------------------------------

}  // namespace epsql::db::lambda::semantic
