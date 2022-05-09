/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <inttypes.h>
#include <string>
#include <vector>

#include "database.hpp"


namespace epsql::db
{

template <typename T>
class Create
{
public:
    Create(const T& value) : created{value} {}
    Create(const Create&) = delete;
    Create& operator=(const Create&) = delete;
    Create(Create&&) = default;
    Create& operator=(Create&&) = default;

public:
    T created{};
};

template <typename T>
class Update
{
public:
    Update(const T& value, std::string primaryKey, std::vector<std::string> changedFieldsList)
        : updated{value}
        , primaryKey{std::move(primaryKey)}
        , changedFieldsList{std::move(changedFieldsList)}
    {}
    Update(const Update&) = delete;
    Update& operator=(const Update&) = delete;
    Update(Update&&) = default;
    Update& operator=(Update&&) = default;

public:
    T updated{};
    std::string primaryKey{};
    std::vector<std::string> changedFieldsList;
};

template <typename T>
class Delete
{
public:
    Delete(const T& value) : deleted{value} {}
    Delete(const Delete&) = delete;
    Delete& operator=(const Delete&) = delete;
    Delete(Delete&&) = default;
    Delete& operator=(Delete&&) = default;

public:
    T deleted{};
};


class Transaction
{
public:
    Transaction(std::weak_ptr<DatabaseTransaction> database)
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

    template <typename Table>
    inline void commit(Delete<Table> record)
    {
        if (auto database = m_database.lock())
            database->remove(std::move(record));
    }

    inline Transaction clone()
    {
        return Transaction{m_database};
    }

private:
    std::weak_ptr<DatabaseTransaction> m_database;
};

}  // namespace epsql::db
