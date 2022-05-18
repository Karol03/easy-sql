/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

#include "db/table/reflectiongroup.hpp"
#include "utils/typeof.hpp"


namespace epsql::db::query::translator
{

template <typename Record>
class CreateRecordQuery
{
public:
    CreateRecordQuery(Record record)
        : m_record{std::move(record)}
    {}

    std::string to_string() const
    {
        const auto tableUuid = utils::TypeOf<decltype(Record::created)>().type();
        const auto tableNameOpt = table::ReflectionRegister::tableName(tableUuid);
        if (!tableNameOpt)
        {
            auto result = std::stringstream{};
            result << "Failed to insert record into table with id '"
                   << tableUuid
                   << "'. No table with given UUID!";
            throw std::logic_error(result.str());
        }

        const auto tableName = *tableNameOpt;
        auto result = std::stringstream{};
        result << "INSERT INTO " << tableName << "\n    ";
        listFilledColumns(result, tableName);
        result << "\n  VALUES\n    ";
        listFilledValues(result);
        addOnConflict(result);
        return result.str();
    }

private:
    void listFilledColumns(std::stringstream& ostr, std::string tableName) const
    {
        const auto& fieldNames = m_record.created.fieldNames();
        if (fieldNames.empty())
        {
            auto result = std::stringstream{};
            result << "Failed to insert record into table '"
                   << tableName
                   << "'. Table has no registered field names!";
            throw std::logic_error(result.str());
        }

        ostr << "(" << fieldNames[0];
        for (auto i = 1ul; i < fieldNames.size(); ++i)
            ostr << ", " << fieldNames[i];
        ostr << ")";
    }

    void listFilledValues(std::stringstream& ostr) const
    {
        const auto& fieldNames = m_record.created.fieldNames();
        const auto& created = m_record.created;

        ostr << "(" << created.getValueAsString(fieldNames[0]);
        for (auto i = 1ul; i < fieldNames.size(); ++i)
            ostr << ", " << created.getValueAsString(fieldNames[i]);
        ostr << ")";
    }

    void addOnConflict(std::stringstream& ostr) const
    {
        const auto primaryKeyName = std::string{m_record.created.primaryKeyName()};
        if (!primaryKeyName.empty())
        {
            ostr << "\n  ON CONFLICT (" << primaryKeyName << ") DO NOTHING";
        }
    }

private:
    Record m_record;
};

}  // namespace epsql::db::query::translator
