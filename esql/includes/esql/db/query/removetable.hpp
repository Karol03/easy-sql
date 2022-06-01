/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "query.hpp"


namespace esql::db::query
{

template <typename Table>
class RemoveTableQuery : public Query
{
public:
    std::string to_string() const override
    {
        return std::string{"DROP TABLE IF EXISTS "} + Table::name() + " CASCADE";
    }
};

}  // namespace esql::db::query
