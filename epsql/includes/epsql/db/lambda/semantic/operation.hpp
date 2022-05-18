/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <memory>
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
    explicit OperationBeetween(std::string tableAndFieldName, std::string min, std::string max);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_min;
    std::string m_max;
};


class OperationNotBeetween : public Operation
{
public:
    explicit OperationNotBeetween(std::string tableAndFieldName, std::string min, std::string max);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_min;
    std::string m_max;
};


class OperationAnyOf : public Operation
{
public:
    explicit OperationAnyOf(std::string tableAndFieldName, std::vector<std::string> range);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::vector<std::string> m_range;
};


class OperationNoneOf : public Operation
{
public:
    explicit OperationNoneOf(std::string tableAndFieldName, std::vector<std::string> range);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::vector<std::string> m_range;
};


class OperationIsNull : public Operation
{
public:
    explicit OperationIsNull(std::string tableAndFieldName);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
};


class OperationIsNotNull : public Operation
{
public:
    explicit OperationIsNotNull(std::string tableAndFieldName);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
};


class OperationStartsWith : public Operation
{
public:
    explicit OperationStartsWith(std::string tableAndFieldName, std::string prefix);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_prefix;
};


class OperationEndsWith : public Operation
{
public:
    explicit OperationEndsWith(std::string tableAndFieldName, std::string postfix);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_postfix;
};


class OperationRegexp : public Operation
{
public:
    explicit OperationRegexp(std::string tableAndFieldName, std::string regexp);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_regexp;
};


class OperationEqual : public Operation
{
public:
    explicit OperationEqual(std::string tableAndFieldName, std::string value);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationDifferentThan : public Operation
{
public:
    explicit OperationDifferentThan(std::string tableAndFieldName, std::string value);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationLess : public Operation
{
public:
    explicit OperationLess(std::string tableAndFieldName, std::string value);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationLessEqual : public Operation
{
public:
    explicit OperationLessEqual(std::string tableAndFieldName, std::string value);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationBigger : public Operation
{
public:
    explicit OperationBigger(std::string tableAndFieldName, std::string value);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationBiggerEqual : public Operation
{
public:
    explicit OperationBiggerEqual(std::string tableAndFieldName, std::string value);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_tableAndFieldName;
    std::string m_value;
};


class OperationFieldEqual : public Operation
{
public:
    explicit OperationFieldEqual(std::string firstTableAndFieldName,
                                 std::string secondTableAndFieldName);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldDifferent : public Operation
{
public:
    explicit OperationFieldDifferent(std::string firstTableAndFieldName,
                                     std::string secondTableAndFieldName);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldBigger : public Operation
{
public:
    explicit OperationFieldBigger(std::string firstTableAndFieldName,
                                  std::string secondTableAndFieldName);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldBiggerEqual : public Operation
{
public:
    explicit OperationFieldBiggerEqual(std::string firstTableAndFieldName,
                                       std::string secondTableAndFieldName);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldLess : public Operation
{
public:
    explicit OperationFieldLess(std::string firstTableAndFieldName,
                                std::string secondTableAndFieldName);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

private:
    std::string m_firstTableAndFieldName;
    std::string m_secondTableAndFieldName;
};


class OperationFieldLessEqual : public Operation
{
public:
    explicit OperationFieldLessEqual(std::string firstTableAndFieldName,
                                     std::string secondTableAndFieldName);
    std::string parse() const override;
    std::vector<std::string> tables() const override;

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
