/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>

#include "query.hpp"


namespace epsql::db::query
{

template <typename Table>
class SizeTableQuery : public Query
{
public:
    std::string to_string() const override
    {
        auto result = std::stringstream{};
        result << "SELECT pg_size_pretty( pg_database_size('" << Table().name() << "') )";
        return result.str();
    }
};

}  // namespace epsql::db::query
