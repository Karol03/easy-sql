/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <iostream>

#include "dbresult.hpp"


namespace epsql::db
{

class Postgres
{
public:
    explicit Postgres() = default;

    bool connect(std::string connectionString)
    {
        std::cout << "[PostgreSQL] Connect \"" << connectionString << "\"\n";
        return true;
    }

    bool disconnect()
    {
        std::cout << "[PostgreSQL] Disconnect\n";
        return true;
    }

    bool isConnected() { return true; }

    DbResult query(std::string sqlQuery)
    {
        std::cerr << "[PostgreSQL] Call query:\n" << sqlQuery << "\n";
        return DbResult{};
    }
};

}  // namespace epsql::db
