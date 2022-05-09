/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>

#include "query.hpp"


namespace epsql::db::query
{

template <typename Table>
class IsExistsTableQuery : public Query
{
public:
    std::string to_string() const override
    {
        auto result = std::stringstream{};
        result << "SELECT count(*) FROM information_schema.tables WHERE table_name = 'remotes'";
        return result.str();
    }
};

}  // namespace epsql::db::query
