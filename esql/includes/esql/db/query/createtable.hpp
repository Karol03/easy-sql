/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include "query.hpp"


namespace esql::db::query
{

template <typename Table>
class CreateTableQuery : public Query
{
public:
    std::string to_string() const override
    {
        if (Table::fieldFullNames().empty())
            throw std::runtime_error{"Cannot create empty table"};

        auto result = std::stringstream{};

        result << "CREATE TABLE IF NOT EXISTS " << Table::name();
        appendFields(result);
        appendPrimaryKey(result);
        appendForeignKeys(result);
        result << ")";

        return result.str();
    }

private:
    inline void appendFields(std::stringstream& stream) const
    {
        const auto fullNames = Table::fieldFullNames();
        stream << " (\n    " << fullNames[0];
        for (auto i = 1u; i < fullNames.size(); ++i)
        {
            stream << ",\n    " << fullNames[i];
        }
    }

    inline void appendPrimaryKey(std::stringstream& stream) const
    {
        const auto fieldNames = Table::fieldNames();
        const auto findKeyInTable = [](const auto& field) { return strcmp(field, Table::primaryKeyName()) == 0; };
        if (strlen(Table::primaryKeyName()) > 0 &&
            std::any_of(fieldNames.cbegin(), fieldNames.cend(), findKeyInTable))
        {
            stream << ",\n  PRIMARY KEY (" << Table::primaryKeyName() << ")";
        }
    }

    inline void appendForeignKeys(std::stringstream& stream) const
    {
        const auto foreignKeys = Table::foreignKeys();
        if (foreignKeys.empty())
            return;

        for (const auto& foreignKey : foreignKeys)
        {
            if (strlen(foreignKey.referenceTable) == 0 ||
                strlen(foreignKey.referenceField) == 0)
                continue;

            stream << ",\n  CONSTRAINT fk_" << foreignKey.field
                   << "\n    FOREIGN KEY(" << foreignKey.field << ")"
                   << "\n    REFERENCES " << foreignKey.referenceTable
                   << '(' << foreignKey.referenceField << ") ON DELETE CASCADE";
        }
    }
};

}  // namespace esql::db::query
