/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


namespace epsql::db::lambda::semantic
{

class OperationType
{
public:
    virtual ~OperationType() = default;

protected:
    inline auto split(std::string tableAndFieldName) const
    {
        auto delimiterPos = tableAndFieldName.find(':');
        if ((delimiterPos == std::string::npos) || (delimiterPos + 2 < tableAndFieldName.size()))
        {
            auto result = std::stringstream{};
            result << "Failed to split table and field in '" << tableAndFieldName << "'";
            throw std::runtime_error{result.str()};
        }

        struct result{
            std::string table;
            std::string field;
        };

        return result{tableAndFieldName.substr(0u, delimiterPos),
                      tableAndFieldName.substr(delimiterPos + 2)};
    }

    inline auto cppFieldNameToSql(std::string tableAndFieldName) const
    {
        auto names = split(std::move(tableAndFieldName));
        names.table += ".";
        names.table += std::move(names.field);
        return names.table;
    }

public:
    std::string parse(std::string tableAndFieldName) const
    {
        return parseOperation(cppFieldNameToSql(tableAndFieldName));
    }

    std::string parseWithoutTableName(std::string tableAndFieldName) const
    {
        const auto names = split(std::move(tableAndFieldName));
        return parseOperation(std::move(names.field));
    }

protected:
    virtual std::string parseOperation(std::string tableAndFieldName) const = 0;
};


class OperationBeetween : public OperationType
{
public:
    explicit OperationBeetween(std::string min, std::string max)
        : min{min}
        , max{max}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName
               << " BEETWEEN " << min << " AND " << max << ")";
        return result.str();
    }

public:
    std::string min, max;
};


class OperationNotBeetween : public OperationType
{
public:
    explicit OperationNotBeetween(std::string min, std::string max)
        : min{min}
        , max{max}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName
               << " NOT BEETWEEN " << min << " AND " << max << ")";
        return result.str();
    }

public:
    std::string min, max;
};


class OperationAnyOf : public OperationType
{
public:
    explicit OperationAnyOf(std::vector<std::string> range)
        : range{std::move(range)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " = ANY (ARRAY["
               << range.front();
        for (auto i = 1ul; i < range.size(); ++i)
            result << ", " << range[i];

        result << "]))";
        return result.str();
    }

public:
    std::vector<std::string> range;
};


class OperationNoneOf : public OperationType
{
public:
    explicit OperationNoneOf(std::vector<std::string> range)
        : range{std::move(range)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " <> ALL (ARRAY["
               << range.front();
        for (auto i = 1ul; i < range.size(); ++i)
            result << ", " << range[i];

        result << "]))";
        return result.str();
    }

public:
    std::vector<std::string> range;
};


class OperationIsNull : public OperationType
{
public:
    explicit OperationIsNull() = default;

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " IS NULL)";
        return result.str();
    }
};


class OperationIsNotNull : public OperationType
{
public:
    explicit OperationIsNotNull() = default;

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " IS NOT NULL)";
        return result.str();
    }
};


class OperationStartsWith : public OperationType
{
public:
    explicit OperationStartsWith(std::string prefix)
        : prefix{std::move(prefix)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " = '" << prefix << "%')";
        return result.str();
    }

public:
    std::string prefix;
};


class OperationEndsWith : public OperationType
{
public:
    explicit OperationEndsWith(std::string postfix)
        : postfix{std::move(postfix)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " = '%" << postfix << "')";
        return result.str();
    }

public:
    std::string postfix;
};


class OperationRegexp : public OperationType
{
public:
    explicit OperationRegexp(std::string regexp)
        : regexp{std::move(regexp)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " = '" << regexp << "')";
        return result.str();
    }

public:
    std::string regexp;
};


class OperationEqual : public OperationType
{
public:
    explicit OperationEqual(std::string value)
        : value{std::move(value)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " = " << value << ")";
        return result.str();
    }

public:
    std::string value;
};


class OperationDifferentThan : public OperationType
{
public:
    explicit OperationDifferentThan(std::string value)
        : value{std::move(value)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " <> " << value << ")";
        return result.str();
    }

public:
    std::string value;
};


class OperationLess : public OperationType
{
public:
    explicit OperationLess(std::string value)
        : value{std::move(value)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " < " << value << ")";
        return result.str();
    }

public:
    std::string value;
};


class OperationLessEqual : public OperationType
{
public:
    explicit OperationLessEqual(std::string value)
        : value{std::move(value)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " <= " << value << ")";
        return result.str();
    }

public:
    std::string value;
};


class OperationBigger : public OperationType
{
public:
    explicit OperationBigger(std::string value)
        : value{std::move(value)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " > " << value << ")";
        return result.str();
    }

public:
    std::string value;
};


class OperationBiggerEqual : public OperationType
{
public:
    explicit OperationBiggerEqual(std::string value)
        : value{std::move(value)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " >= " << value << ")";
        return result.str();
    }

public:
    std::string value;
};


class OperationFieldEqual : public OperationType
{
public:
    explicit OperationFieldEqual(std::string secondTableAndFieldName)
        : secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " = "
               << cppFieldNameToSql(secondTableAndFieldName) << ")";
        return result.str();
    }

public:
    std::string secondTableAndFieldName;
};


class OperationFieldDifferent : public OperationType
{
public:
    explicit OperationFieldDifferent(std::string secondTableAndFieldName)
        : secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " <> "
               << cppFieldNameToSql(secondTableAndFieldName) << ")";
        return result.str();
    }

public:
    std::string secondTableAndFieldName;
};


class OperationFieldBigger : public OperationType
{
public:
    explicit OperationFieldBigger(std::string secondTableAndFieldName)
        : secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " > "
               << cppFieldNameToSql(secondTableAndFieldName) << ")";
        return result.str();
    }

public:
    std::string secondTableAndFieldName;
};


class OperationFieldBiggerEqual : public OperationType
{
public:
    explicit OperationFieldBiggerEqual(std::string secondTableAndFieldName)
        : secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " >= "
               << cppFieldNameToSql(secondTableAndFieldName) << ")";
        return result.str();
    }

public:
    std::string secondTableAndFieldName;
};


class OperationFieldLess : public OperationType
{
public:
    explicit OperationFieldLess(std::string secondTableAndFieldName)
        : secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " < "
               << cppFieldNameToSql(secondTableAndFieldName) << ")";
        return result.str();
    }

public:
    std::string secondTableAndFieldName;
};


class OperationFieldLessEqual : public OperationType
{
public:
    explicit OperationFieldLessEqual(std::string secondTableAndFieldName)
        : secondTableAndFieldName{std::move(secondTableAndFieldName)}
    {}

protected:
    std::string parseOperation(std::string tableAndFieldName) const override
    {
        auto result = std::stringstream{};
        result << "(" << tableAndFieldName << " <= "
               << cppFieldNameToSql(secondTableAndFieldName) << ")";
        return result.str();
    }

public:
    std::string secondTableAndFieldName;
};


template <typename T, typename... Args>
inline auto make(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}


template <typename T, typename... Args>
inline auto makeOppositeOperation(Args&&... args)
{
    if constexpr (std::is_same_v<T, OperationFieldBigger>)
        return make<OperationFieldLess>(std::forward<Args>(args)...);
    else if constexpr (std::is_same_v<T, OperationFieldBiggerEqual>)
        return make<OperationFieldLessEqual>(std::forward<Args>(args)...);
    else if constexpr (std::is_same_v<T, OperationFieldDifferent>)
        return make<OperationFieldEqual>(std::forward<Args>(args)...);
    else if constexpr (std::is_same_v<T, OperationFieldEqual>)
        return make<OperationFieldDifferent>(std::forward<Args>(args)...);
    else if constexpr (std::is_same_v<T, OperationFieldLess>)
        return make<OperationFieldBigger>(std::forward<Args>(args)...);
    else if constexpr (std::is_same_v<T, OperationFieldLessEqual>)
        return make<OperationFieldBiggerEqual>(std::forward<Args>(args)...);
}

}  // namespace epsql::db::lambda::semantic
