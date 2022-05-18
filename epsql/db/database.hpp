/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <memory>
#include <vector>

#include "query/queries.hpp"
#include "query/translator/createrecord.hpp"
#include "query/translator/removerecord.hpp"
#include "query/translator/updaterecord.hpp"
#include "dbresult.hpp"


namespace epsql::db
{

class IDatabaseConnection
{
public:
    virtual ~IDatabaseConnection() = default;

    virtual bool connect(std::string connectionString) = 0;
    virtual bool disconnect() = 0;
    virtual bool isConnected() = 0;
};


class IDatabaseTransaction
{
public:
    virtual ~IDatabaseTransaction() = default;

    virtual bool clean() = 0;
    virtual bool push() = 0;
    virtual DbResult query(std::string sqlQuery) = 0;

protected:
    std::string m_commits;
};


class DatabaseTransaction : public IDatabaseTransaction
{
public:
    template <typename Table>
    inline bool isExists() { return query(query::IsExistsTableQuery<Table>{}.to_string()).isResultTrue(); }

    template <typename Table>
    inline bool create() { return query(query::CreateTableQuery<Table>{}.to_string()).isSuccess(); }

    template <typename Table>
    inline bool remove() { return query(query::RemoveTableQuery<Table>{}.to_string()).isSuccess(); }

    template <typename Table>
    inline std::size_t size() { return query(query::SizeTableQuery<Table>{}.to_string()).template get<std::size_t>(); }

    template <typename RecordCreate>
    inline bool create(RecordCreate record)
    {
        if (!m_commits.empty())
            m_commits += ";\n";
        m_commits += query::translator::CreateRecordQuery{std::move(record)}.to_string();
        return true;
    }

    template <typename RecordUpdate>
    inline bool update(RecordUpdate record)
    {
        if (!m_commits.empty())
            m_commits += ";\n";
        m_commits += query::translator::UpdateRecordQuery(std::move(record)).to_string();
        return true;
    }

    template <typename RecordDelete>
    inline bool remove(RecordDelete record)
    {
        if (!m_commits.empty())
            m_commits += ";\n";
        m_commits += query::translator::RemoveRecordQuery(std::move(record)).to_string();
        return true;
    }
};



template <typename SQL>
class Database : public IDatabaseConnection,
                 public DatabaseTransaction,
                 public std::enable_shared_from_this<Database<SQL>>
{
private:
    explicit Database() = default;

public:
    inline bool connect(std::string connectionString) override { return m_sql.connect(std::move(connectionString)); }
    inline bool disconnect() override { return m_sql.disconnect(); }
    inline bool isConnected() override { return m_sql.isConnected(); }

    inline DbResult query(std::string sqlQuery) override { return m_sql.query(std::move(sqlQuery)); }
    inline bool clean() override { m_commits.clear(); return true; }
    inline bool push() override
    {
        auto queryPayload = std::string{};
        std::swap(queryPayload, m_commits);
        return query(queryPayload).isSuccess();
    }

    template <typename Table>
    inline std::vector<Table> findFirst(std::string queryPayload, std::size_t limit)
    {
        if (!queryPayload.empty())
            queryPayload += " LIMIT " + std::to_string(limit);
        return query(std::move(queryPayload)).template get<std::vector<Table>>();
    }

    template <typename Table>
    inline std::vector<Table> findLast(std::string queryPayload, std::size_t limit)
    {
        if (!queryPayload.empty())
            queryPayload += " DESC LIMIT " + std::to_string(limit);
        return query(std::move(queryPayload)).template get<std::vector<Table>>();
    }

    template <typename Table>
    inline std::vector<Table> findAll(std::string queryPayload)
    {
        return query(std::move(queryPayload)).template get<std::vector<Table>>();
    }


public:
    inline static std::shared_ptr<Database<SQL>> createInstance()
    {
        auto database = Database<SQL>{};
        return std::make_shared<Database<SQL>>(std::move(database));
    }

private:
    SQL m_sql;
};

}  // namespace epsql::db
