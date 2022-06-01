/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <string>

#include "dbresult.hpp"


namespace esql::db
{

class Postgres
{
public:
    explicit Postgres() = default;
    ~Postgres();

    bool connect(std::string connectionString);
    bool disconnect();
    bool isConnected();
    DbResult query(std::string sqlQuery);
};

}  // namespace esql::db
