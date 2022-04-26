/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <functional>
#include <string>
#include <variant>


namespace epsql::db::lambda
{

struct Query
{
    Query(std::string query)
        : m_query{std::move(query)}
    {}

    Query(std::function<std::string()> query)
        : m_query{std::move(query)}
    {}

    inline operator std::string()
    {
        return std::visit([this](auto&& value) { return get(std::move(value)); }, m_query);
    }

    inline void operator()(std::string table)
    {
        std::swap(m_table, table);
    }

private:
    inline std::string get(std::string&& value) { return std::move(value); }
    inline std::string get(std::function<std::string()>&& value) { return value(); }

private:
    std::variant<std::string, std::function<std::string()>> m_query;
    std::string m_table;
};

}  // namespace epsql::db::lambda
