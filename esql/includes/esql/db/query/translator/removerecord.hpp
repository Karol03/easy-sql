/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>
#include <string>

#include "esql/db/table/reflectiongroup.hpp"


namespace esql::db::query::translator
{

template <typename Record>
class RemoveRecordQuery
{
public:
    RemoveRecordQuery(Record record)
        : m_record{std::move(record)}
    {}

    std::string to_string() const
    {
        const auto tableUuid = utils::TypeOf<decltype(Record::deleted)>().type();
        const auto tableNameOpt = table::ReflectionRegister::tableName(tableUuid);
        if (!tableNameOpt)
        {
            auto result = std::stringstream{};
            result << "Failed to insert record into table with id '"
                   << tableUuid
                   << "'. No table for given UUID!";
            throw std::logic_error(result.str());
        }

        auto result = std::stringstream{};
        result << "DELETE FROM " << *tableNameOpt << " WHERE "
               << m_record.deleted.primaryKeyName()
               << " = " << getPrimaryKeyValue();
        return result.str();
    }

private:
    std::string getPrimaryKeyValue() const
    {
        return m_record.deleted.getValueAsString(m_record.deleted.primaryKeyName());
    }

private:
    Record m_record;
};

}  // namespace esql::db::query::translator
