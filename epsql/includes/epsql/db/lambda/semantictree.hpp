/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <string>
#include <unordered_set>

#include "semantic/operationtree.hpp"


namespace epsql::db::lambda
{

struct SemanticTree
{
public:
    explicit SemanticTree(semantic::OperationTree tree);
    std::string build(std::string tableName, std::string primaryKeyName);

private:
    std::pair<std::size_t, std::size_t> countStatements();

    std::string buildMixedStatements(std::string tableName, std::string primaryKeyName);
    std::string buildNoStatements(std::string tableName, std::string primaryKeyName);
    std::string buildOnlyAndStatements(std::string tableName, std::string primaryKeyName);
    std::string buildOnlyOrStatements(std::string tableName, std::string primaryKeyName);

    std::string selectAll(std::string tableName, std::string primaryKeyName);
    std::string selectOneTableRelated(std::string tableName,
                                      std::string primaryKeyName,
                                      std::unordered_set<std::string> tables);
    std::string selectOnlyMainTable(std::string tableName, std::string primaryKeyName);
    std::string selectTableNotRelated(std::string tableName,
                                      std::string primaryKeyName,
                                      std::unordered_set<std::string> tables);

    std::unordered_set<std::string> getTables();

private:
    semantic::OperationTree m_tree;
};

}  // namespace epsql::db::lambda
