/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>


namespace epsql::db::lambda::semantic
{

class Operation
{
public:
    virtual ~Operation() = default;

public:
    virtual std::string parse() const = 0;
    virtual std::vector<std::string> tables() const = 0;
};


class OperationBeetween : public Operation
{
public:
    explicit OperationBeetween(std::string tableAndFieldName, std::string min, std::string max)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_min{std::move(min)}
        , m_max{std::move(max)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " BEETWEEN " << m_min << " AND " << m_max << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_min;
    std::string m_max;
};


class OperationNotBeetween : public Operation
{
public:
    explicit OperationNotBeetween(std::string tableAndFieldName, std::string min, std::string max)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_min{std::move(min)}
        , m_max{std::move(max)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " NOT BEETWEEN " << m_min << " AND " << m_max << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_min;
    std::string m_max;
};


class OperationAnyOf : public Operation
{
public:
    explicit OperationAnyOf(std::string tableAndFieldName, std::vector<std::string> range)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_range{std::move(range)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " = ANY (ARRAY[" << m_range.front();

        for (auto i = 1ul; i < m_range.size(); ++i)
            result << ", " << m_range[i];

        result << "]))";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::vector<std::string> m_range;
};


class OperationNoneOf : public Operation
{
public:
    explicit OperationNoneOf(std::string tableAndFieldName, std::vector<std::string> range)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_range{std::move(range)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " <> ALL (ARRAY[" << m_range.front();

        for (auto i = 1ul; i < m_range.size(); ++i)
            result << ", " << m_range[i];

        result << "]))";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::vector<std::string> m_range;
};


class OperationIsNull : public Operation
{
public:
    explicit OperationIsNull(std::string tableAndFieldName)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " IS NULL)";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
};


class OperationIsNotNull : public Operation
{
public:
    explicit OperationIsNotNull(std::string tableAndFieldName)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " IS NOT NULL)";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
};


class OperationStartsWith : public Operation
{
public:
    explicit OperationStartsWith(std::string tableAndFieldName, std::string prefix)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_prefix{std::move(prefix)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " = '" << m_prefix << "%')";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_prefix;
};


class OperationEndsWith : public Operation
{
public:
    explicit OperationEndsWith(std::string tableAndFieldName, std::string postfix)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_postfix{std::move(postfix)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " = '%" << m_postfix << "')";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_postfix;
};


class OperationRegexp : public Operation
{
public:
    explicit OperationRegexp(std::string tableAndFieldName, std::string regexp)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_regexp{std::move(regexp)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " = '" << m_regexp << "')";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_regexp;
};


class OperationEqual : public Operation
{
public:
    explicit OperationEqual(std::string tableAndFieldName, std::string value)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_value{std::move(value)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " = " << m_value << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationDifferentThan : public Operation
{
public:
    explicit OperationDifferentThan(std::string tableAndFieldName, std::string value)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_value{std::move(value)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " <> " << m_value << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationLess : public Operation
{
public:
    explicit OperationLess(std::string tableAndFieldName, std::string value)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_value{std::move(value)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " < " << m_value << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationLessEqual : public Operation
{
public:
    explicit OperationLessEqual(std::string tableAndFieldName, std::string value)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_value{std::move(value)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " <= " << m_value << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationBigger : public Operation
{
public:
    explicit OperationBigger(std::string tableAndFieldName, std::string value)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_value{std::move(value)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " > " << m_value << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationBiggerEqual : public Operation
{
public:
    explicit OperationBiggerEqual(std::string tableAndFieldName, std::string value)
        : m_tableAndFieldName{std::move(tableAndFieldName)}
        , m_value{std::move(value)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_tableAndFieldName << " >= " << m_value << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_tableAndFieldName.substr(0, m_tableAndFieldName.find('.'))};
    }

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationFieldEqual : public Operation
{
public:
    explicit OperationFieldEqual(std::string firstTableAndFieldName,
                                 std::string secondTableAndFieldName)
        : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
        , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_firstTableAndFieldName << " = "
               << m_secondTableAndFieldName << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
                m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
    }

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldDifferent : public Operation
{
public:
    explicit OperationFieldDifferent(std::string firstTableAndFieldName,
                                     std::string secondTableAndFieldName)
        : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
        , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

public:
    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_firstTableAndFieldName << " <> "
               << m_secondTableAndFieldName << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
                m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
    }

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldBigger : public Operation
{
public:
    explicit OperationFieldBigger(std::string firstTableAndFieldName,
                                  std::string secondTableAndFieldName)
        : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
        , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

public:
    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_firstTableAndFieldName << " > "
               << m_secondTableAndFieldName << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
                m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
    }

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldBiggerEqual : public Operation
{
public:
    explicit OperationFieldBiggerEqual(std::string firstTableAndFieldName,
                                       std::string secondTableAndFieldName)
        : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
        , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

public:
    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_firstTableAndFieldName << " >= "
               << m_secondTableAndFieldName << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
                m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
    }

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldLess : public Operation
{
public:
    explicit OperationFieldLess(std::string firstTableAndFieldName,
                                std::string secondTableAndFieldName)
        : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
        , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

public:
    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_firstTableAndFieldName << " < "
               << m_secondTableAndFieldName << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
                m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
    }

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldLessEqual : public Operation
{
public:
    explicit OperationFieldLessEqual(std::string firstTableAndFieldName,
                                     std::string secondTableAndFieldName)
        : m_firstTableAndFieldName{std::move(firstTableAndFieldName)}
        , m_secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

public:
    std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_firstTableAndFieldName << " <= "
               << m_secondTableAndFieldName << ")";
        return result.str();
    }

    std::vector<std::string> tables() const override
    {
        return {m_firstTableAndFieldName.substr(0, m_firstTableAndFieldName.find('.')),
                m_secondTableAndFieldName.substr(0, m_secondTableAndFieldName.find('.'))};
    }

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


template <typename T, typename... Args>
inline auto makeOperation(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace epsql::db::lambda::semantic
