/**
 * Created by Karol Dudzic @ 2022
 */

#include "epsql/db/lambda/semantictree.hpp"

#include <sstream>
#include <stdexcept>
#include <iostream>

#include "epsql/db/table/reflectiongroup.hpp"


namespace epsql::db::lambda
{

SemanticTree::SemanticTree(semantic::OperationTree tree)
    : m_tree{std::move(tree)}
{}

std::string SemanticTree::build(std::string tableName, std::string primaryKeyName)
{
    m_tree.validate();

    auto [andStatements, orStatements] = countStatements();

    if (andStatements == 0 && orStatements == 0)
        return buildNoStatements(std::move(tableName), std::move(primaryKeyName));
    else if (andStatements == 0)
        return buildOnlyOrStatements(std::move(tableName), std::move(primaryKeyName));
    else if (orStatements == 0)
        return buildOnlyAndStatements(std::move(tableName), std::move(primaryKeyName));
    else
        return buildMixedStatements(std::move(tableName), std::move(primaryKeyName));
}

std::pair<std::size_t, std::size_t> SemanticTree::countStatements()
{
    auto ands = 0ul;
    auto ors = 0ul;

    const auto countStatement = [&ands, &ors](const auto& operationResult)
    {
        if (operationResult.type() == semantic::EOperationResultType::And)
            ++ands;
        else if (operationResult.type() == semantic::EOperationResultType::Or)
            ++ors;
    };

    m_tree.walk(countStatement);

    return {ands, ors};
}

std::string SemanticTree::buildNoStatements(std::string tableName, std::string primaryKeyName)
{
    const auto tables = getTables();
    if (tables.size() == 0ul)
    {
        return selectAll(std::move(tableName), std::move(primaryKeyName));
    }

    if (tables.find(tableName) != tables.end())
    {
        if (tables.size() == 1ul)
            return selectOnlyMainTable(std::move(tableName), std::move(primaryKeyName));
        else
            return selectOneTableRelated(std::move(tableName),
                                         std::move(primaryKeyName),
                                         std::move(tables));
    }
    else
    {
        return selectTableNotRelated(std::move(tableName),
                                     std::move(primaryKeyName),
                                     std::move(tables));
    }
}

std::string SemanticTree::selectAll(std::string tableName, std::string primaryKeyName)
{
    auto result = std::stringstream{};
    result << "SELECT * FROM " << tableName
           << "\nGROUP BY " << primaryKeyName;
    return result.str();
}

std::string SemanticTree::selectOnlyMainTable(std::string tableName, std::string primaryKeyName)
{
    const auto conditions = m_tree.main().parse();
    auto result = std::stringstream{};
    result << "SELECT * FROM " << tableName
           << "\nWHERE " << conditions
           << "\nGROUP BY " << primaryKeyName;
    return result.str();
}

std::string SemanticTree::selectOneTableRelated(std::string tableName,
                                                std::string primaryKeyName,
                                                std::unordered_set<std::string> tables)
{
    tables.erase(tableName);

    const auto conditions = m_tree.main().parse();
    auto result = std::stringstream{};
    result << "SELECT " << tableName << ".* FROM " << tableName << ", " << *tables.begin()
           << "\nWHERE " << conditions
           << "\nGROUP BY " << tableName << '.' << primaryKeyName;
    return result.str();
}

std::string SemanticTree::selectTableNotRelated(std::string tableName,
                                                std::string primaryKeyName,
                                                std::unordered_set<std::string> tables)
{
    auto relationConditions = std::vector<std::pair<std::string, std::string>>{};

    for (const auto& table : tables)
    {
        try
        {
            relationConditions = table::ForeignKeyReflectionGroup::findTablesRelationPath(tableName, table);
            if (!relationConditions.empty())
                break;
        }
        catch (std::exception&) {}
    }

    if (relationConditions.empty())
        return selectAll(std::move(tableName), std::move(primaryKeyName));

    auto relationConditionResult = std::stringstream{};
    relationConditionResult << "((" << relationConditions.front().first
                            << " = " << relationConditions.front().second << ")";
    for (auto i = 1ul; i < relationConditions.size(); ++i)
        relationConditionResult << " AND (" << relationConditions[i].first
                                << " = " << relationConditions[i].second << ")";
    relationConditionResult << ")";

    const auto conditions = m_tree.main().parse();
    auto result = std::stringstream{};
    result << "SELECT " << tableName << ".* FROM " << tableName;

    for (const auto& table : tables)
        result << ", " << table;

    result << "\nWHERE " << relationConditionResult.str()
           << " AND\n      " << conditions
           << "\nGROUP BY " << tableName << '.' << primaryKeyName;
    return result.str();
}

std::string SemanticTree::buildOnlyOrStatements(std::string tableName, std::string primaryKeyName)
{
    throw std::logic_error{"buildOnlyOrStatements not implemented"};
    const auto tables = getTables();
    return {};
}

std::string SemanticTree::buildOnlyAndStatements(std::string tableName, std::string primaryKeyName)
{
    throw std::logic_error{"buildOnlyAndStatements not implemented"};
    const auto tables = getTables();
    return {};
}

std::string SemanticTree::buildMixedStatements(std::string tableName, std::string primaryKeyName)
{
    throw std::logic_error{"buildMixedStatements not implemented"};
    return {};
}

std::unordered_set<std::string> SemanticTree::getTables()
{
    auto result = std::unordered_set<std::string>{};

    const auto takeTables = [&result](const auto& operationResult)
    {
        auto tables = operationResult.tables();
        result.insert(tables.begin(), tables.end());
    };

    m_tree.walk(takeTables);

    return result;
}

}  // namespace epsql::db::lambda
