/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>
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

struct IPAddress
{
    IPAddress() : m_address{} {}
    IPAddress(std::string address) : m_address{std::move(address)} {}
    IPAddress(uint32_t address) { set(address); }
    IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { set(a, b, c, d); }

    IPAddress(const IPAddress&) = default;
    IPAddress& operator=(const IPAddress&) = default;
    IPAddress(IPAddress&&) = default;
    IPAddress& operator=(IPAddress&&) = default;

    inline operator bool() const { return isValid(); }
    inline bool isValid() const { return !m_address.empty(); }
    inline void set(const std::string& value) { m_address = value;}
    inline void set(const uint32_t value) { set(value >> 24, (value >> 16) & 0xFF, (value >> 8) & 0xFF, (value >> 24)); }
    inline void set(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
    {
        m_address = std::to_string(uint16_t{a});
        m_address += "." + std::to_string(uint16_t{b});
        m_address += "." + std::to_string(uint16_t{c});
        m_address += "." + std::to_string(uint16_t{d});
    }

    inline std::string to_string() const { return m_address; }

private:
    std::string m_address;
};


struct Credentials
{
public:
    explicit Credentials() = default;
    explicit Credentials(std::string databaseName)
        : m_databaseName{std::move(databaseName)}
    {}
    explicit Credentials(std::string databaseName, std::string userName)
        : m_databaseName{std::move(databaseName)}
        , m_userName{std::move(userName)}
    {}
    explicit Credentials(std::string databaseName, std::string userName, std::string userPassword)
        : m_databaseName{std::move(databaseName)}
        , m_userName{std::move(userName)}
        , m_userPassword{std::move(userPassword)}
    {}
    explicit Credentials(std::string databaseName, std::string userName,
                         std::string userPassword, IPAddress host)
        : m_databaseName{std::move(databaseName)}
        , m_userName{std::move(userName)}
        , m_userPassword{std::move(userPassword)}
        , m_address{std::move(host)}
    {}
    explicit Credentials(std::string databaseName, std::string userName,
                         std::string userPassword, IPAddress host, uint16_t port)
        : m_databaseName{std::move(databaseName)}
        , m_userName{std::move(userName)}
        , m_userPassword{std::move(userPassword)}
        , m_address{std::move(host)}
        , m_port{port}
    {}

    inline Credentials& setDatabase(const std::string& name) { m_databaseName = name; return *this; }
    inline Credentials& setUser(const std::string& name) { m_userName = name; return *this; }
    inline Credentials& setPassword(const std::string& password) { m_userPassword = password; return *this; }
    inline Credentials& setHostAddress(const IPAddress& address) { m_address = address; return *this; }
    inline Credentials& setHostAddress(const std::string& address) { m_address = IPAddress{address}; return *this; }
    inline Credentials& setPort(uint16_t port) { m_port = port; return *this; }
    inline Credentials& setPort(const std::string& port) { m_port = std::atoi(port.c_str()); return *this; }

    inline std::string getDatabase() const { return m_databaseName; }
    inline std::string getUser() const { return m_userName; }
    inline std::string getPassword() const { return m_userPassword; }
    inline std::string getHostAddress() const { return m_address.to_string(); }
    inline uint16_t getPort() const { return m_port; }

    inline operator bool() const { return isValid(); }
    inline bool isValid() const { return m_address.isValid() && !m_databaseName.empty() && !m_userName.empty() && m_port > 0; }

    std::string to_string() const
    {
        auto result = std::stringstream{};
        result << "dbname = " << m_databaseName << " user = " << m_userName
               << " password = " << m_userPassword << " hostaddr = " << m_address.to_string()
               << " port = " << std::to_string(m_port);
        return result.str();
    }

private:
    std::string m_databaseName{};
    std::string m_userName{};
    std::string m_userPassword{};
    IPAddress m_address{};
    uint16_t m_port{};
};


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

    SQL(Credentials credentials)
        : m_database{db::Database<SQLType>::createInstance()}
        , m_errorStream{std::make_shared<SinkStream>()}
    {
        if (credentials.isValid())
            connect(std::move(credentials.to_string()));
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

    inline bool connect(Credentials credentials)
    {
        try
        {
            if (isConnected())
                return true;
            else if (!credentials.isValid())
                throw std::runtime_error{"Credentials are invalid, connection impossible"};
            else
                return m_database->connect(std::move(credentials.to_string()));
        }
        catch(std::exception& e)
        {
            (*m_errorStream) << "ERROR: " << e.what() << '\n';
            return false;
        }
    }

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
