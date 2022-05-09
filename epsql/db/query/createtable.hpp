/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include "query.hpp"


namespace epsql::db::query
{

template <typename Table>
class CreateTableQuery : public Query
{
public:
    std::string to_string() const override
    {
        auto table = Table();
        if (table.fieldFullNames().empty())
            throw std::runtime_error{"Cannot create empty table"};

        auto result = std::stringstream{};

        result << "CREATE TABLE IF NOT EXISTS " << table.name();
        appendFields(table, result);
        appendPrimaryKey(table, result);
        appendForeignKeys(table, result);
        result << ")";

        return result.str();
    }

private:
    inline void appendFields(const Table& table, std::stringstream& stream) const
    {
        stream << " (\n    " << table.fieldFullNames()[0];
        for (auto i = 1u; i < table.fieldFullNames().size(); ++i)
        {
            stream << ",\n    " << table.fieldFullNames()[i];
        }
    }

    inline void appendPrimaryKey(const Table& table, std::stringstream& stream) const
    {
        auto findKeyInTable = [&table](const auto& field) { return strcmp(field, table.primaryKeyName()) == 0; };
        if (strlen(table.primaryKeyName()) > 0 &&
            std::any_of(table.fieldNames().cbegin(), table.fieldNames().cend(), findKeyInTable))
        {
            stream << ",\n  PRIMARY KEY (" << table.primaryKeyName() << ")";
        }
    }

    inline void appendForeignKeys(const Table& table, std::stringstream& stream) const
    {
        if (table.foreignKeys().empty())
            return;

        for (const auto& foreignKey : table.foreignKeys())
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

}  // namespace epsql::db::query
