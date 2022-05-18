/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>
#include <stdexcept>

#include "semantic/operationtree.hpp"


namespace epsql::db::lambda
{

struct SemanticTree
{
public:
    explicit SemanticTree(semantic::OperationTree tree)
        : m_tree{std::move(tree)}
    {}

    std::string build(std::string tableName, std::string primaryKeyName)
    {
//        if (!m_tree.isOperationsFor(tableName))
//            std::runtime_error{"Cannot fetch objects from table without any field comparasions!"};

//        if (!m_tree.tableReferences(tableName))
//        {
//            return selectWithoutTableDependencies(std::move(tableName),
//                                                  std::move(primaryKeyName));
//        }
//        else
//        {
//            return selectWithTableDependencies(std::move(tableName),
//                                               std::move(primaryKeyName));
//        }
        return {};
    }

private:
//    std::string selectWithoutTableDependencies(std::string tableName,
//                                               std::string primaryKeyName)
//    {
//        m_tree.discardNotRelated(tableName);

//        auto result = std::stringstream{};
//        result << "SELECT * FROM " << tableName
//               << "\nWHERE " << m_tree.walkTreeFor(tableName)
//               << "\nGROUP BY " << primaryKeyName;
//        return result.str();
//    }

//    std::string selectWithTableDependencies(std::string tableName,
//                                            std::string primaryKeyName)
//    {
//        m_tree.discardNotRelated(tableName);

//        auto result = std::stringstream{};
//        result << "SELECT " << tableName << ".* FROM " << tableName;

//        for (const auto& table : tableName.relationChain())
//        {
//            result << "JOIN " << table << " ON "
//                   <<

//        }

//        result << "\nGROUP BY " << tableName << '.' << primaryKeyName;
//        return result.str();
//    }

private:
    semantic::OperationTree m_tree;
};

}  // namespace epsql::db::lambda
