/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <string>


namespace epsql::db::query
{

class Query
{
public:
    virtual ~Query() = default;
    virtual std::string to_string() const = 0;
};

}  // namespace epsql::db::query
