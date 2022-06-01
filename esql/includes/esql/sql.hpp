/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <string>
#include <type_traits>

#include "db/database.hpp"
#include "db/lambda.hpp"
#include "db/core.hpp"
#include "db/record.hpp"
#include "db/table/table.hpp"
#include "db/table/types.hpp"
#include "db/transaction.hpp"
#include "errorstream.hpp"


namespace esql
{

template <typename SQLType>
class SQL
{
public:
    SQL()
        : m_database{db::Database<SQLType>::createInstance()}
        , m_errorStream{std::make_shared<SinkStream>()}
    {}

    SQL(std::string connectionString)
        : m_database{db::Database<SQLType>::createInstance()}
        , m_errorStream{std::make_shared<SinkStream>()}
    {
        connect(std::move(connectionString));
    }

    template <typename ErrorStream, typename = std::enable_if_t<std::is_base_of_v<std::ostream, ErrorStream>>>
    SQL(std::shared_ptr<ErrorStream> errorStream)
        : m_database{db::Database<SQLType>::createInstance()}
        , m_errorStream{std::move(errorStream)}
    {}

    template <typename ErrorStream, typename = std::enable_if_t<std::is_base_of_v<std::ostream, ErrorStream>>>
    SQL(std::string connectionString, std::shared_ptr<ErrorStream> errorStream)
        : m_database{db::Database<SQLType>::createInstance()}
        , m_errorStream{std::move(errorStream)}
    {
        connect(std::move(connectionString));
    }

    SQL(const SQL&) = delete;
    SQL& operator=(const SQL&) = delete;
    SQL(SQL&&) = default;
    SQL& operator=(SQL&&) = default;

    ~SQL() { disconnect(); }

    inline bool connect(std::string connectionString)
    {
        try
        {
            if (!isConnected())
                return m_database->connect(std::move(connectionString));
            return true;
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    inline bool disconnect()
    {
        try
        {
            if (isConnected())
                return m_database->disconnect();
            return true;
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    inline bool isConnected()
    {
        try
        {
            return m_database->isConnected();
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    inline bool push()
    {
        try
        {
            return m_database->push();
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    inline bool clean()
    {
        try
        {
            return m_database->clean();
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    inline bool query(std::string sqlQuery)
    {
        try
        {
            return m_database->query(std::move(sqlQuery)).isSuccess();
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>, bool> isExists()
    {
        try
        {
            return m_database->template isExists<Table>();
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>, bool> create()
    {
        try
        {
            return m_database->template create<Table>();
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>, bool> remove()
    {
        try
        {
            return m_database->template remove<Table>();
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>, std::size_t> size()
    {
        try
        {
            return m_database->template size<Table>();
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return 0ull;
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>,
    db::Record<Table>> insert()
    {
        try
        {
            auto result = db::Record<Table>(openTransaction());
            result.create();
            return result;
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return db::Record<Table>(openInvalidTransaction());
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>,
    db::Record<Table>> find(db::lambda::Query query)
    {
        try
        {
            query(Table::name(), Table::primaryKeyName());
            auto first = m_database->template findFirst<Table>(query, 1ul);
            if (!first.empty())
                return db::Record<Table>(openTransaction(), std::move(first.front()));
            else
                return db::Record<Table>(openInvalidTransaction());
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return db::Record<Table>(openInvalidTransaction());
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>,
    std::vector<db::Record<Table>>> find(db::lambda::Query query, std::size_t limit)
    {
        try
        {
            query(Table::name(), Table::primaryKeyName());
            auto result = std::vector<db::Record<Table>>{};
            for (auto&& element : m_database->template findFirst<Table>(query, limit))
            {
                result.push_back(db::Record<Table>(openTransaction(), std::move(element)));
            }
            return result;
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return {};
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>,
    db::Record<Table>> findLast(db::lambda::Query query)
    {
        try
        {
            query(Table::name(), Table::primaryKeyName());
            auto last = m_database->template findLast<Table>(query, 1ul);
            if (!last.empty())
                return db::Record<Table>(openTransaction(), std::move(last.front()));
            else
                return db::Record<Table>(openInvalidTransaction());
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return db::Record<Table>(openInvalidTransaction());
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>,
    std::vector<db::Record<Table>>> findLast(db::lambda::Query query, std::size_t limit)
    {
        try
        {
            query(Table::name(), Table::primaryKeyName());
            auto result = std::vector<db::Record<Table>>{};
            for (auto&& element : m_database->template findLast<Table>(query, limit))
            {
                result.push_back(db::Record<Table>(openTransaction(), std::move(element)));
            }
            return result;
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return {};
        }
    }

    template <typename Table>
    std::enable_if_t<std::is_base_of_v<db::table::ITable, Table>,
    std::vector<db::Record<Table>>> findAll(db::lambda::Query query)
    {
        try
        {
            query(Table::name(), Table::primaryKeyName());
            auto result = std::vector<db::Record<Table>>{};
            for (auto&& element : m_database->template findAll<Table>(query))
            {
                result.push_back(db::Record<Table>(openTransaction(), std::move(element)));
            }
            return result;
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return {};
        }
    }

private:
    inline db::Transaction openTransaction()
    {
        return db::Transaction{m_database->weak_from_this()};
    }

    inline db::Transaction openInvalidTransaction()
    {
        auto database = db::Database<SQLType>::createInstance();
        return db::Transaction{database->weak_from_this()};
    }

private:
    std::shared_ptr<db::Database<SQLType>> m_database;
    std::shared_ptr<std::ostream> m_errorStream;
};


using PostgreSQL = SQL<db::Postgres>;

}  // namespace esql
