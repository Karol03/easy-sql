/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <optional>
#include <string>
#include <unordered_set>
#include <unordered_set>
#include <vector>

#include "semantic/operationtree.hpp"


namespace esql::db::lambda
{

struct SemanticTree
{
public:
    explicit SemanticTree(semantic::OperationTree tree);
    std::string build(std::string tableName, std::string primaryKeyName);

private:
    std::optional<std::string> missingRelation(
            const std::unordered_set<std::string>& tables,
            std::unordered_map<std::string, std::unordered_set<std::string>> relationChain);
    std::pair<std::string, std::vector<std::string>> findAnyPath(std::string beginTable, std::vector<std::string> possibleEndTables);
    std::optional<std::string> gatherRelationsFrom(
            std::unordered_set<std::string>& resultTab,
            const std::unordered_set<std::string>& tables,
            std::unordered_map<std::string, std::unordered_set<std::string>>& relationChain);
    std::optional<std::string> gatherRelationsFrom(
            std::unordered_set<std::string>& resultTab,
            const std::unordered_set<std::string>& tables);
    std::pair<std::size_t, std::size_t> countConstants();
    std::string mergePath(std::vector<std::pair<std::string, std::string>> path);
    std::unordered_map<std::string, std::vector<std::string>> collectTableRelations();
    std::unordered_set<std::string> getTables();
    std::vector<std::pair<std::string, std::string>> findFirstPath();
    std::vector<std::string> parseMultiplePathTree();

    std::string selectAll();
    std::string selectOnlyMainTable();
    std::string selectTableNotRelated();
    std::string selectMultipleTablesRelation();

private:
    semantic::OperationTree m_tree;
    std::unordered_set<std::string> m_tables;
    std::string m_tableName;
    std::string m_primaryKeyName;
};

}  // namespace esql::db::lambda
