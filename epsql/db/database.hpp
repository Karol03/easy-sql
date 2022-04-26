/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "utils/typeof.hpp"


namespace epsql::db
{

class Database : public std::enable_shared_from_this<Database>
{
private:
    explicit Database() = default;

public:
    bool connect(std::string connectionString)
    {
        std::cerr << "[DATABASE] Connect \"" << connectionString << "\"\n";
        return true;
    }

    bool disconnect()
    {
        std::cerr << "[DATABASE] Disconnect\n";
        return true;
    }

    bool isConnected()
    {
        std::cerr << "[DATABASE] Is connected\n";
        return true;
    }

    bool push()
    {
        std::cerr << "[DATABASE] Push\n";
        return true;
    }

    bool clean()
    {
        std::cerr << "[DATABASE] Clean\n";
        return true;
    }

    template <typename Table>
    bool isExists()
    {
        std::cerr << "[DATABASE] Is table \"" << utils::TypeOf<Table>().type() << "\" exists\n";
        return true;
    }

    template <typename Table>
    bool create()
    {
        std::cerr << "Create table {" << utils::TypeOf<Table>().type() << "}\n";
        return true;
    }

    template <typename Table>
    bool remove()
    {
        std::cerr << "Remove table {" << utils::TypeOf<Table>().type() << "}\n";
        return true;
    }

    template <typename Table>
    std::size_t size()
    {
        std::cerr << "Size of table\n";
        return 0ull;
    }

    template <typename Record>
    bool create(Record record)
    {
        std::cerr << "Create record for table\n";
        return true;
    }

    template <typename Record>
    bool update(Record record)
    {
        std::cerr << "Update record for table\n";
        return true;
    }

    bool remove(uint64_t recordId, uint64_t tableId)
    {
        std::cerr << "Remove record {" << recordId << "} for table {" << tableId << "}\n";
        return true;
    }

    template <typename Table>
    std::vector<Table> findFirst(std::string query, std::size_t limit)
    {
        std::cerr << "Looking for [" << limit << "] objects '" << query << "'\n";
        return {};
    }

    template <typename Table>
    std::vector<Table> findLast(std::string query, std::size_t limit)
    {
        std::cerr << "Looking for last [" << limit << "] objects '" << query << "'\n";
        return {};
    }

    template <typename Table>
    std::vector<Table> findAll(std::string query)
    {
        std::cerr << "Looking for all '" << query << "'\n";
        return {};
    }


public:
    inline static std::shared_ptr<Database> create()
    {
        auto database = Database{};
        return std::make_shared<Database>(std::move(database));
    }
};

}  // namespace epsql::db
