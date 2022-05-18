/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <functional>
#include <string>

#include "semantictree.hpp"


namespace epsql::db::lambda
{

struct Query
{
    explicit Query(std::function<SemanticTree()> query);
    operator std::string();
    void operator()(std::string table, std::string primaryKey);

private:
    std::function<SemanticTree()> m_queryCreator;
    std::string m_table;
    std::string m_primaryKey;
};

}  // namespace epsql::db::lambda
