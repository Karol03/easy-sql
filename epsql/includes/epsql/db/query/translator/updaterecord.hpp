/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

#include "epsql/db/table/reflectiongroup.hpp"
#include "epsql/utils/typeof.hpp"


namespace epsql::db::query::translator
{

template <typename Record>
class UpdateRecordQuery
{
public:
    UpdateRecordQuery(Record record)
        : m_record{std::move(record)}
    {}

    std::string to_string() const
    {
        const auto tableUuid = utils::TypeOf<decltype(Record::updated)>().type();
        const auto tableNameOpt = table::ReflectionRegister::tableName(tableUuid);
        if (!tableNameOpt)
        {
            auto result = std::stringstream{};
            result << "Failed to update record into table with id '"
                   << tableUuid
                   << "'. No table with given UUID!";
            throw std::logic_error(result.str());
        }

        const auto tableName = *tableNameOpt;
        auto result = std::stringstream{};
        result << "UPDATE " << tableName;
        listSetFields(result);
        setWhereCondition(result);
        return result.str();
    }

private:
    void listSetFields(std::stringstream& result) const
    {
        const auto& object = m_record.updated;
        const auto& changedFieldsList = m_record.changedFieldsList;

        result << "\n  SET " << changedFieldsList[0]
               << " = " << object.getValueAsString(changedFieldsList[0].c_str());

        for (auto i = 1ul; i < changedFieldsList.size(); ++i)
        {
            const auto& fieldName = changedFieldsList[i].c_str();
            result << ",\n      " << fieldName << " = " << object.getValueAsString(fieldName);
        }
    }

    void setWhereCondition(std::stringstream& result) const
    {
        result << "\n  WHERE " << m_record.updated.primaryKeyName() << " = " << m_record.primaryKey;
    }

private:
    Record m_record;
};

}  // namespace epsql::db::query::translator
