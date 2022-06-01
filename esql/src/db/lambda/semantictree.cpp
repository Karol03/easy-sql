/**
 * Created by Karol Dudzic @ 2022
 */

#include "esql/db/lambda/semantictree.hpp"

#include <cassert>
#include <sstream>
#include <stdexcept>

#include "esql/db/table/reflectiongroup.hpp"


namespace esql::db::lambda
{
namespace
{
constexpr auto SELECT_NOTHING = "";
}  // namespace

SemanticTree::SemanticTree(semantic::OperationTree tree)
    : m_tree{std::move(tree)}
{}

std::string SemanticTree::build(std::string tableName, std::string primaryKeyName)
{
    m_tree.validate();

    const auto [falseResults, trueResults] = countConstants();

    std::swap(m_tableName, tableName);
    std::swap(m_primaryKeyName, primaryKeyName);
    m_tables = getTables();

    if (m_tables.empty())
    {
        if (falseResults != 0)
        {
            return SELECT_NOTHING;
        }
        else if (trueResults != 0)
        {
            return selectAll();
        }
        else
        {
            throw std::runtime_error{"Cannot find proper query for given lambda 'false == true == 0'"};
        }
    }
    else if (m_tables.size() == 1ul)
    {
        if (m_tables.find(m_tableName) != m_tables.end())
        {
            return selectOnlyMainTable();
        }
        else
        {
            return selectTableNotRelated();
        }
    }
    else
    {
        return selectMultipleTablesRelation();
    }
}

std::pair<std::size_t, std::size_t> SemanticTree::countConstants()
{
    auto falseResults = 0ul;
    auto trueResults = 0ul;

    const auto countConstant = [&falseResults, &trueResults](const auto& operationResult)
    {
        if (operationResult.type() == semantic::EOperationResultType::Contant_false)
            ++falseResults;
        else if (operationResult.type() == semantic::EOperationResultType::Contant_true)
            ++trueResults;
    };

    m_tree.walk(countConstant);

    return {falseResults, trueResults};
}

std::vector<std::pair<std::string, std::string>> SemanticTree::findFirstPath()
{
    auto result = std::vector<std::pair<std::string, std::string>>{};
    assert(m_tables.size() == 1ul);
    const auto& endTable = *m_tables.begin();

    try
    {
        result = table::ForeignKeyReflectionGroup::findTablesRelationPath(m_tableName, endTable);
        if (!result.empty())
            return result;
    }
    catch (std::exception&) {}
    return {};
}

std::pair<std::string, std::vector<std::string>> SemanticTree::findAnyPath(
        std::string beginTable,
        std::vector<std::string> possibleEndTables)
{
    auto result = table::ForeignKeyReflectionGroup::findTablesRelationPath(std::move(beginTable),
                                                                           std::move(possibleEndTables));
    if (result.empty())
        return {};

    auto visitedTables = std::unordered_set<std::string>{};
    for (const auto& [first, second] : result)
    {
        visitedTables.insert(first);
        visitedTables.insert(second);
    }

    auto visited = std::vector<std::string>{};
    visited.reserve(visitedTables.size());
    for (const auto& table : visitedTables)
        visited.push_back(table.substr(0u, table.find('.')));

    auto path = mergePath(std::move(result));
    return {path, visited};
}

std::vector<std::string> SemanticTree::parseMultiplePathTree()
{
    const auto chains = m_tree.squash();

    auto expressions = std::vector<std::string>{};
    auto usedTables = std::unordered_set<std::string>{};
    auto expressionTables = std::unordered_set<std::string>{};
    auto relationChain = std::unordered_map<std::string, std::unordered_set<std::string>>{};

    for (const auto& chain : chains)
    {
        expressionTables.clear();
        relationChain.clear();

        expressionTables.insert(m_tableName);

        for (const auto& [firstTab, secondTab] : chain.connections)
        {
            expressionTables.insert(firstTab);
            if (!secondTab.empty())
            {
                expressionTables.insert(secondTab);

                if (relationChain.count(firstTab))
                    relationChain[firstTab].insert(secondTab);
                else
                    relationChain[secondTab].insert(firstTab);
            }
        }

        usedTables.insert(std::make_move_iterator(expressionTables.begin()),
                          std::make_move_iterator(expressionTables.end()));

        auto chainMissingRelation = missingRelation(expressionTables, std::move(relationChain));
        if (chainMissingRelation)
        {
            auto expression = chain.expression + std::move(*chainMissingRelation);
            expressions.push_back(std::move(expression));
        }
    }

    if (expressions.empty())
        throw std::logic_error{"The relationship between all required arrays was not found. The result of the query will always be empty."};

    return expressions;
}

std::optional<std::string> SemanticTree::missingRelation(
        const std::unordered_set<std::string>& tables,
        std::unordered_map<std::string, std::unordered_set<std::string>> relationChain)
{
    auto foundTables = std::unordered_set<std::string>{m_tableName};

    auto result = gatherRelationsFrom(foundTables, tables, relationChain);
    if (!result)
        return result;

    if (foundTables.size() != tables.size())
    {
        auto singleResult = gatherRelationsFrom(foundTables, tables);
        if (!singleResult)
            return std::nullopt;

        *result += std::move(*singleResult);
    }

    return result;
}

std::optional<std::string> SemanticTree::gatherRelationsFrom(
        std::unordered_set<std::string>& resultTab,
        const std::unordered_set<std::string>& tables)
{
    auto result = std::string{};
    auto knownTables = resultTab;

    while (resultTab.size() != tables.size())
    {
        auto isChanged = false;
        for (const auto& table : tables)
        {
            if (resultTab.count(table) == 0)
            {
                auto [missingRelation, missingTables] = findAnyPath(table, std::vector(knownTables.begin(), knownTables.end()));
                if (!missingRelation.empty())
                {
                    result += " AND " + std::move(missingRelation);
                    for (const auto& missingTable : missingTables)
                    {
                        if (tables.count(missingTable))
                            resultTab.insert(missingTable);
                    }
                    knownTables.insert(std::make_move_iterator(missingTables.begin()),
                                       std::make_move_iterator(missingTables.end()));

                    isChanged = true;
                    break;
                }
            }
        }

        if (!isChanged)
            return std::nullopt;
    }
    return result;
}

std::optional<std::string> SemanticTree::gatherRelationsFrom(
        std::unordered_set<std::string>& resultTab,
        const std::unordered_set<std::string>& tables,
        std::unordered_map<std::string, std::unordered_set<std::string>>& relationChain)
{
    auto result = std::string{};
    const auto isInResult = [&resultTab](const auto& relation) { return resultTab.count(relation); };
    auto isChange = false;

    while (!relationChain.empty())
    {
        for (auto& [table, relations] : relationChain)
        {
            if (resultTab.count(table))
            {
                resultTab.insert(std::make_move_iterator(relations.begin()),
                                 std::make_move_iterator(relations.end()));
                relationChain.erase(table);
                isChange = true;
                break;
            }
            else if (std::any_of(relations.begin(), relations.end(), isInResult))
            {
                resultTab.insert(std::move(table));
                resultTab.insert(std::make_move_iterator(relations.begin()),
                                 std::make_move_iterator(relations.end()));
                relationChain.erase(table);
                isChange = true;
                break;
            }
        }

        if (!isChange)
        {
            for (auto& [table, relations] : relationChain)
            {
                auto [missingRelation, missingTables] = findAnyPath(table, std::vector(resultTab.begin(), resultTab.end()));
                if (!missingRelation.empty())
                {
                    result += " AND " + std::move(missingRelation);
                    resultTab.insert(std::move(table));
                    resultTab.insert(std::make_move_iterator(relations.begin()),
                                     std::make_move_iterator(relations.end()));
                    relationChain.erase(table);
                    for (const auto& missingTable : missingTables)
                    {
                        if (tables.count(missingTable))
                            resultTab.insert(missingTable);
                    }
                    isChange = true;
                    break;
                }

                for (const auto& relation : relations)
                {
                    auto [missingRelation, missingTables] = findAnyPath(relation, std::vector(resultTab.begin(), resultTab.end()));
                    if (!missingRelation.empty())
                    {
                        result += " AND " + std::move(missingRelation);
                        resultTab.insert(std::move(table));
                        resultTab.insert(std::make_move_iterator(relations.begin()),
                                         std::make_move_iterator(relations.end()));
                        relationChain.erase(table);
                        isChange = true;
                        for (const auto& missingTable : missingTables)
                        {
                            if (tables.count(missingTable))
                                resultTab.insert(missingTable);
                        }
                        break;
                    }
                }

                if (isChange)
                    break;
            }
        }

        if (!isChange)
            return std::nullopt;

        isChange = false;
    }
    return result;
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

std::unordered_map<std::string, std::vector<std::string>> SemanticTree::collectTableRelations()
{
    auto result = std::unordered_map<std::string, std::vector<std::string>>{};

    const auto takeTables = [&result](const auto& operationResult)
    {
        if (operationResult.type() != semantic::EOperationResultType::Data)
        {
            return;
        }

        const auto tables = operationResult.tables();
        if (tables.size() == 2ul)
        {
            result[tables.front()].push_back(tables.back());
            result[tables.back()].push_back(tables.front());
        };
    };

    m_tree.walk(takeTables);

    return result;
}

std::string SemanticTree::mergePath(std::vector<std::pair<std::string, std::string>> path)
{
    auto result = std::stringstream{};
    if (path.size() == 1ul)
    {
        result << '(' << path.front().first << " = " << path.front().second << ')';
    }
    else
    {
        result << "((" << path.front().first << " = " << path.front().second << ')';
        for (auto i = 1ul; i < path.size(); ++i)
            result << " AND (" << path[i].first << " = " << path[i].second << ')';
        result << ')';
    }
    return result.str();
}

std::string SemanticTree::selectAll()
{
    auto result = std::stringstream{};
    result << "SELECT * FROM " << m_tableName
           << "\nGROUP BY " << m_primaryKeyName;
    return result.str();
}

std::string SemanticTree::selectOnlyMainTable()
{
    const auto conditions = m_tree.main().parse();
    auto result = std::stringstream{};
    result << "SELECT * FROM " << m_tableName
           << "\nWHERE " << conditions
           << "\nGROUP BY " << m_primaryKeyName;
    return result.str();
}

std::string SemanticTree::selectTableNotRelated()
{
    const auto tableRelations = collectTableRelations();

    auto relationConditionsPath = findFirstPath();
    if (relationConditionsPath.empty())
    {
        auto result = std::stringstream{};
        result << "No relation found between '" << m_tableName << "' AND '" << *m_tables.begin() << "'";
        throw std::invalid_argument{result.str()};
    }

    const auto relationConditions = mergePath(std::move(relationConditionsPath));
    const auto conditions = m_tree.main().parse();
    auto result = std::stringstream{};

    result << "SELECT " << m_tableName << ".* FROM " << m_tableName;

    for (const auto& table : m_tables)
        result << ", " << table;

    result << "\nWHERE " << relationConditions
           << " AND\n      " << conditions
           << "\nGROUP BY " << m_tableName << '.' << m_primaryKeyName;
    return result.str();
}

std::string SemanticTree::selectMultipleTablesRelation()
{
    const auto relationConditions = parseMultiplePathTree();
    auto result = std::stringstream{};

    auto it = m_tables.begin();
    result << "SELECT " << m_tableName << ".* FROM " << *it;

    while ((++it) != m_tables.end())
        result << ", " << *it;

    auto relationIt = relationConditions.begin();
    result << "\nWHERE (" << *relationIt << ")";

    while ((++relationIt) != relationConditions.end())
        result << " ||\n      (" << *relationIt << ")";

    result << "\nGROUP BY " << m_tableName << '.' << m_primaryKeyName;
    return result.str();
}

}  // namespace esql::db::lambda
