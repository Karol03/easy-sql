/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <inttypes.h>

#include "database.hpp"


namespace epsql::db
{

template <typename T>
class Create
{
public:
    Create(T& value) : created{&value} {}

public:
    T* created;
};

template <typename T>
class Update
{
public:
    Update(T& value) : updated{&value} {}

public:
    T* updated;
};

class Delete
{
public:
    uint64_t id{};
    uint64_t tableId{};
};


class Transaction
{
public:
    Transaction(std::weak_ptr<Database> database)
        : m_database{database}
    {}

    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    Transaction(Transaction&&) = default;
    Transaction& operator=(Transaction&&) = default;

    // add filter for fields
    template <typename Table>
    inline void commit(Create<Table> record)
    {
        if (auto database = m_database.lock())
            database->create(std::move(record));
    }

    template <typename Table>
    inline void commit(Update<Table> record)
    {
        if (auto database = m_database.lock())
            database->update(std::move(record));
    }

    inline void commit(Delete record)
    {
        if (auto database = m_database.lock())
            database->remove(record.id, record.tableId);
    }

    inline Transaction clone()
    {
        return Transaction{m_database};
    }

private:
    std::weak_ptr<Database> m_database;
};

}  // namespace epsql::db
