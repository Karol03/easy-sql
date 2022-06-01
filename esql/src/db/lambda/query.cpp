/**
 * Created by Karol Dudzic @ 2022
 */
#include "esql/db/lambda/query.hpp"

#include <functional>
#include <stdexcept>
#include <string>

#include "esql/db/lambda/semantictree.hpp"


namespace esql::db::lambda
{

Query::Query(std::function<SemanticTree()> query)
    : m_queryCreator{std::move(query)}
{}

Query::operator std::string()
{
    if (!m_queryCreator)
        throw std::runtime_error{"Query without body is forbidden!"};

    auto semanticTree = m_queryCreator();
    return semanticTree.build(m_table, m_primaryKey);
}

void Query::operator()(std::string table, std::string primaryKey)
{
    std::swap(m_table, table);
    std::swap(m_primaryKey, primaryKey);
}

}  // namespace esql::db::lambda
