/**
 * Created by Karol Dudzic @ 2022
 */

#include "esql/db/table/reflectiongroup.hpp"

#include <queue>
#include <stdexcept>


namespace esql::db::table
{

// ForeignKeyReflectionGroup::Tree
void ForeignKeyReflectionGroup::Tree::insert(std::string tableName,
                                             std::string fieldName,
                                             std::string referenceTable,
                                             std::string referenceField)
{
    m_relations[tableName][referenceTable].insert(fieldName + '.' + referenceField);
    m_relations[referenceTable][tableName].insert(referenceField + '.' + fieldName);
}

std::vector<std::pair<std::string, std::string>> ForeignKeyReflectionGroup::Tree::pathStraight(
        std::string beginTableName,
        std::string endTableName)
{
    validateBranchesExist(beginTableName, endTableName);
    return pathFinding(beginTableName, {endTableName});
}

std::vector<std::pair<std::string, std::string>> ForeignKeyReflectionGroup::Tree::pathMultiple(
        std::string beginTableName,
        std::vector<std::string> possibleEndTables)
{
    auto filteredPossibleEndTables = std::vector<std::string>{};
    for (auto& endTableName : possibleEndTables)
    {
        if (silentValidateBranchesExist(beginTableName, endTableName))
            filteredPossibleEndTables.push_back(std::move(endTableName));
    }

    if (filteredPossibleEndTables.empty())
        return {};

    auto result = pathFinding(beginTableName, filteredPossibleEndTables);
    if (!result.empty())
        return result;
    return {};
}

std::vector<std::pair<std::string, std::string>> ForeignKeyReflectionGroup::Tree::pathFinding(
        const std::string& beginTableName,
        const std::vector<std::string>& possibleEndTables)
{
    auto path = std::unordered_map<std::string, std::string>{};
    auto visited = std::unordered_set<std::string>{};
    auto toVisit = std::queue<std::string>{};
    toVisit.push(std::move(beginTableName));
    auto endTableName = std::string{};

    while (!toVisit.empty())
    {
        auto tableName = toVisit.front();
        toVisit.pop();

        if (!visited.insert(tableName).second)
            continue;

        const auto isContainsTable = [this, &tableName, &endTableName](const auto& possibleEndTableName)
        {
            if (m_relations[tableName].find(possibleEndTableName) == m_relations[tableName].end())
                return false;
            endTableName = possibleEndTableName;
            return true;
        };

        if (std::any_of(possibleEndTables.begin(), possibleEndTables.end(), isContainsTable))
        {
            path[endTableName] = tableName;
            return composeResult(std::move(path), std::move(endTableName));
        }
        else
        {
            for (const auto& relation : m_relations[tableName])
            {
                if (path.count(tableName) && path[tableName] == relation.first)
                    continue;
                path[relation.first] = tableName;
                toVisit.push(relation.first);
            }
        }
    }
    return {};
}

std::vector<std::pair<std::string, std::string>> ForeignKeyReflectionGroup::Tree::composeResult(
        std::unordered_map<std::string, std::string> path,
        std::string endTableName)
{
    auto result = std::vector<std::pair<std::string, std::string>>{};

    for (auto tableName = path[endTableName];
         !tableName.empty();
         endTableName = tableName, tableName = path[endTableName])
    {
        auto relatedFields = *m_relations[tableName][endTableName].begin();
        auto firstRelatedField = relatedFields.substr(0, relatedFields.find('.'));
        auto secondRelatedField = relatedFields.substr(relatedFields.find('.') + 1);

        auto firstTableFieldName = tableName + '.' + firstRelatedField;
        auto secondTableFieldName = endTableName + '.' + secondRelatedField;

        result.push_back({std::move(firstTableFieldName),
                          std::move(secondTableFieldName)});
    }

    return std::vector(result.rbegin(), result.rend());
}

std::string ForeignKeyReflectionGroup::Tree::relationFor(
        const std::string& tableName,
        const std::string& endTableName,
        const FieldHintsType& fieldHints)
{
    const auto it = fieldHints.find(tableName);
    if (it == fieldHints.end())
        return {};

    for (const auto& relatedFields : m_relations[tableName][endTableName])
    {
        for (const auto& hintField : it->second)
        {
            if (relatedFields.substr(0, relatedFields.find('.')) == hintField)
            {
                return relatedFields;
            }
        }
    }

    return {};
}

void ForeignKeyReflectionGroup::Tree::validateBranchesExist(const std::string& beginName,
                                                            const std::string& endName) const
{
    if (m_relations.find(beginName) == m_relations.end())
    {
        auto result = std::stringstream{};
        result << "No foreign key tied \"" << beginName
               << "\" with any other tables";
        throw std::runtime_error{result.str()};
    }

    if (m_relations.find(endName) == m_relations.end())
    {
        auto result = std::stringstream{};
        result << "No foreign key tied \"" << endName
               << "\" with any other tables";
        throw std::runtime_error{result.str()};
    }
}

bool ForeignKeyReflectionGroup::Tree::silentValidateBranchesExist(const std::string& beginName,
                                                                  const std::string& endName) const
{
    if (m_relations.find(beginName) == m_relations.end())
    {
        return false;
    }

    if (m_relations.find(endName) == m_relations.end())
    {
        return false;
    }

    return true;
}

}  // namespace esql::db::table
