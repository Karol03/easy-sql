/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <functional>
#include <stdexcept>
#include <string>

#include "semantictree.hpp"


namespace epsql::db::lambda
{

struct Query
{
    explicit Query(std::function<SemanticTree()> query)
        : m_queryCreator{std::move(query)}
    {}

    inline operator std::string()
    {
        if (!m_queryCreator)
            throw std::runtime_error{"Query without body is forbidden!"};

        auto semanticTree = m_queryCreator();
        return semanticTree.build(m_table, m_primaryKey);
    }

    inline void operator()(std::string table, std::string primaryKey)
    {
        std::swap(m_table, table);
        std::swap(m_primaryKey, primaryKey);
    }

private:
    std::function<SemanticTree()> m_queryCreator;
    std::string m_table;
    std::string m_primaryKey;
};

}  // namespace epsql::db::lambda
