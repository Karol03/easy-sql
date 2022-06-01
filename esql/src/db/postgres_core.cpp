/**
 * Created by Karol Dudzic @ 2022
 */
#include "esql/db/core.hpp"

#include <iostream>


namespace esql::db
{

Postgres::~Postgres()
{

}

bool Postgres::connect(std::string connectionString)
{
    std::cout << "[PostgreSQL] Connect \"" << connectionString << "\"\n";
    return true;
}

bool Postgres::disconnect()
{
    std::cout << "[PostgreSQL] Disconnect\n";
    return true;
}

bool Postgres::isConnected()
{
    return true;
}

DbResult Postgres::query(std::string sqlQuery)
{
    std::cerr << "[PostgreSQL] Call query:\n" << sqlQuery << "\n";
    return DbResult{};
}

}  // namespace esql::db
