/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "query.hpp"


namespace epsql::db::query
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

}  // namespace epsql::db::query
