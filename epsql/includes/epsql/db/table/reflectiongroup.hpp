/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "epsql/db/value/ivalue.hpp"
#include "epsql/utils/hash.hpp"
#include "types.hpp"


namespace epsql::db::table
{

class ReflectionRegister
{
public:
    virtual ~ReflectionRegister() = default;

    inline static std::optional<std::string> tableName(uint64_t uuid)
    {
        if (m_tableReferenceUuids.find(uuid) != m_tableReferenceUuids.end())
            return m_tableReferenceUuids[uuid];
        return std::nullopt;
    }

    inline static std::optional<uint32_t> reference(const std::string& fieldFullName)
    {
        if (m_referenceHashes.find(fieldFullName) != m_referenceHashes.end())
            return m_referenceHashes[fieldFullName];
        return std::nullopt;
    }

    inline static std::optional<std::string> reference(const uint32_t& fieldUuid)
    {
        if (m_referenceUuids.find(fieldUuid) != m_referenceUuids.end())
            return m_referenceUuids[fieldUuid];
        return std::nullopt;
    }

protected:
    template <typename ClassUuid, typename ClassName, typename... FieldNames>
    inline static void assignAll(ClassUuid uuid, ClassName className,
                                 FieldNames&&... fieldFullNames)
    {
        (assign(fieldFullNames), ...);
        m_tableReferenceUuids[uuid] = className;
    }

    inline static void assign(const char* fieldFullName)
    {
        const auto uuid = utils::Hash::hash(fieldFullName);
        if (m_referenceHashes.find(fieldFullName) == m_referenceHashes.end())
        {
            checkUuidUniqueness(fieldFullName, uuid);
            insertFieldReference(fieldFullName, uuid);
        }
    }

    inline static void checkUuidUniqueness(const char* fieldFullName, const uint32_t uuid)
    {
        if (m_referenceUuids.find(uuid) != m_referenceUuids.end())
        {
            const auto result = std::string{"Cannot create field '"} + fieldFullName +
                                "' the same hash existed for '" +
                                m_referenceUuids[uuid] + "'";
            throw std::logic_error{result.c_str()};
        }
    }

    inline static void insertFieldReference(const char* fieldFullName, const uint32_t uuid)
    {
        m_referenceHashes[fieldFullName] = uuid;
        m_referenceUuids[uuid] = fieldFullName;
    }

private:
    inline static std::unordered_map<std::string, uint32_t> m_referenceHashes = {};
    inline static std::unordered_map<uint32_t, std::string> m_referenceUuids = {};

    inline static std::unordered_map<uint64_t, std::string> m_tableReferenceUuids = {};
};


class ForeignKeyReflectionGroup
{
public:
    using FieldHintsType = std::unordered_map<std::string, std::unordered_set<std::string>>;

protected:
    struct Tree
    {
    private:
        using FieldHintsType = std::unordered_map<std::string, std::unordered_set<std::string>>;
        using RelatedFields = std::unordered_set<std::string>;
        using ConnectedTable = std::unordered_map<std::string, RelatedFields>;
        using TableRelations = std::unordered_map<std::string, ConnectedTable>;

    public:
        inline void insert(std::string tableName,
                           std::string fieldName,
                           std::string referenceTable,
                           std::string referenceField)
        {
            m_relations[tableName][referenceTable].insert(fieldName + '.' + referenceField);
            m_relations[referenceTable][tableName].insert(referenceField + '.' + fieldName);
        }

        std::vector<std::pair<std::string, std::string>> pathWithoutHints(
                std::string beginTableName,
                std::string endTableName)
        {
            validateBranchesExist(beginTableName, endTableName);

            auto path = std::unordered_map<std::string, std::string>{};
            auto visited = std::unordered_set<std::string>{};
            auto toVisit = std::queue<std::string>{};
            toVisit.push(std::move(beginTableName));

            while (!toVisit.empty())
            {
                auto tableName = toVisit.front();
                toVisit.pop();

                if (!visited.insert(tableName).second)
                    continue;

                if (m_relations[tableName].find(endTableName) != m_relations[tableName].end())
                {
                    path[endTableName] = tableName;
                    return composeResult(std::move(path), std::move(endTableName));
                }
                else
                {
                    for (const auto& relation : m_relations[tableName])
                    {
                        path[relation.first] = tableName;
                        toVisit.push(relation.first);
                    }
                }
            }

            return {};
        }

        std::vector<std::pair<std::string, std::string>> pathWithHints(
                std::string beginTableName,
                std::string endTableName,
                std::vector<std::string> tableHints,
                FieldHintsType fieldHints)
        {
            validateBranchesExist(beginTableName, endTableName);

            auto path = std::unordered_map<std::string, std::string>{};
            auto visited = std::unordered_set<std::string>{};
            auto container = std::deque(std::make_move_iterator(tableHints.begin()),
                                        std::make_move_iterator(tableHints.end()));
            container.push_front(std::move(beginTableName));
            auto toVisit = std::queue(std::move(container));

            while (!toVisit.empty())
            {
                auto tableName = toVisit.front();
                toVisit.pop();

                if (!visited.insert(tableName).second)
                    continue;

                if (m_relations[tableName].find(endTableName) != m_relations[tableName].end())
                {
                    path[endTableName] = tableName;
                    if (fieldHints.empty())
                        return composeResult(std::move(path), std::move(endTableName));
                    else
                        return composeResultWithHint(std::move(path),
                                                     std::move(endTableName),
                                                     std::move(fieldHints));
                }
                else
                {
                    for (const auto& relation : m_relations[tableName])
                    {
                        path[relation.first] = tableName;
                        toVisit.push(relation.first);
                    }
                }
            }

            return {};
        }

    private:
        std::vector<std::pair<std::string, std::string>> composeResult(
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

        std::vector<std::pair<std::string, std::string>> composeResultWithHint(
                std::unordered_map<std::string, std::string> path,
                std::string endTableName,
                FieldHintsType fieldHints)
        {
            auto result = std::vector<std::pair<std::string, std::string>>{};

            for (auto tableName = path[endTableName];
                 !tableName.empty();
                 endTableName = tableName, tableName = path[endTableName])
            {
                auto relatedFieldsPair = relationFor(tableName, endTableName, fieldHints);

                if (relatedFieldsPair.empty())
                    relatedFieldsPair = relationFor(endTableName, tableName, fieldHints);

                if (relatedFieldsPair.empty())
                    relatedFieldsPair = *m_relations[tableName][endTableName].begin();

                auto firstRelatedField = relatedFieldsPair.substr(0, relatedFieldsPair.find('.'));
                auto secondRelatedField = relatedFieldsPair.substr(relatedFieldsPair.find('.') + 1);

                auto firstTableFieldName = tableName + '.' + firstRelatedField;
                auto secondTableFieldName = endTableName + '.' + secondRelatedField;

                result.push_back({std::move(firstTableFieldName),
                                  std::move(secondTableFieldName)});
            }

            return std::vector(result.rbegin(), result.rend());
        }

        std::string relationFor(
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

        void validateBranchesExist(const std::string& beginName, const std::string& endName) const
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

    private:
        TableRelations m_relations;
    };

public:
    virtual ~ForeignKeyReflectionGroup() = default;

    static inline std::vector<std::pair<std::string, std::string>> findTablesRelationPath(
            std::string beginTableName,
            std::string endTableName,
            std::vector<std::string> tableHints = {},
            FieldHintsType fieldHints = {})
    {
        if (beginTableName == endTableName)
            return {};
        else if (tableHints.empty() && fieldHints.empty())
            return m_tree.pathWithoutHints(std::move(beginTableName),
                                           std::move(endTableName));
        else
            return m_tree.pathWithHints(std::move(beginTableName),
                                        std::move(endTableName),
                                        std::move(tableHints),
                                        std::move(fieldHints));
    }

protected:
    inline static Tree m_tree = {};
};


template <typename T>
class ForeignKeyReflectionGroupT : public ForeignKeyReflectionGroup
{
protected:
    static inline void insertForeignKey(std::string fieldName,
                                        std::string referenceTable,
                                        std::string referenceField)
    {
        if (!fieldName.empty() && !referenceTable.empty() && !referenceField.empty())
            m_tree.insert(T::name(),
                          std::move(fieldName),
                          std::move(referenceTable),
                          std::move(referenceField));
    }
};


template <typename T>
class ReflectionGroup : public ReflectionRegister, public ForeignKeyReflectionGroupT<T>
{
public:
    static inline uint64_t typeOf(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it == m_names.end())
            return 0ull;

        const auto itPos = it - m_names.begin();
        return m_types[itPos];
    }

    template <typename U>
    inline U* ptr(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it == m_names.end())
            return nullptr;

        const auto itPos = it - m_names.begin();
        return reinterpret_cast<value::ValueT<U>*>(m_position[itPos])->ptr();
    }

    template <typename U>
    inline const U* ptr(const char* name) const
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it == m_names.end())
            return nullptr;

        const auto itPos = it - m_names.begin();
        return reinterpret_cast<value::ValueT<U>*>(m_position[itPos])->ptr();
    }

    template <typename U>
    inline U& get(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it == m_names.end())
        {
            auto result = std::string{"There is no field named '"};
            result += name;
            result += "' in table '";
            result += *tableName(utils::TypeOf<T>().type());
            result += "'";
            throw std::runtime_error{result};
        }

        const auto itPos = it - m_names.begin();
        return *(reinterpret_cast<value::ValueT<U>*>(m_position[itPos])->nonEmptyPtr());
    }

    inline std::string getValueAsString(const char* name) const
    {
        auto result = std::string{};
        const auto value = var(name);
        const auto valueGetter = [&result](auto&& value) { result = table::getValue(value); };
        std::visit(valueGetter, value);
        return result;
    }

    inline TableVariant var(const char* name) const
    {
        const auto type = typeOf(name);

        if (type == 0ull)
            return nullptr;
        if (auto* result = retrive<Smallint>(type, name))
            return *result;
        if (auto* result = retrive<Int>(type, name))
            return *result;
        if (auto* result = retrive<Bigint>(type, name))
            return *result;
        if (auto* result = retrive<Real>(type, name))
            return *result;
        if (auto* result = retrive<Float8>(type, name))
            return *result;
        if (auto* result = retrive<Boolean>(type, name))
            return *result;
        if (auto* result = retrive<Text>(type, name))
            return *result;
        if (auto* result = retrive<Date>(type, name))
            return *result;
        if (auto* result = retrive<Interval>(type, name))
            return *result;
        if (auto* result = retrive<Time>(type, name))
            return *result;
        if (auto* result = retrive<Timestamp>(type, name))
            return *result;
        return nullptr;
    }

    template <typename U>
    inline void set(const char* name, const U& value)
    {
        if (auto* field = get<U>(name))
            *field = value;
    }

    inline void setNull(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it != m_names.end())
        {
            const auto itPos = it - m_names.begin();
            ((value::Value*)m_position[itPos])->setNull();
        }
    }

    inline static const auto& fieldNames() { return m_names; }

protected:
    template <typename... Field>
    ReflectionGroup(Field&&... field)
        : m_position{((void*)&field)...}
        , m_type{utils::TypeOf<std::remove_reference_t<std::remove_cv_t<Field>>>().type()...}
    {}

    template <typename... FieldName>
    inline void reflectNames(FieldName&&... name)
    {
        if (m_position.size() != m_names.size())
            m_names = std::vector{name...};
    }

    template <typename... FieldName>
    inline void reflectTypes(FieldName&&... types)
    {
        if (m_position.size() != m_types.size())
            m_types = std::vector{types...};
    }

private:
    template <typename TableType>
    const TableType* retrive(uint64_t type, const char* name) const
    {
        if (type == utils::TypeOf<TableType>().type())
        {
            if (const auto* result = ptr<TableType>(name))
                return result;
        }
        return nullptr;
    }

protected:
    static inline bool isReferenced = {false};

private:
    std::vector<void*> m_position = {};
    std::vector<std::size_t> m_type = {};

    static inline std::vector<const char*> m_names = {};
    static inline std::vector<uint64_t> m_types = {};
};

}  // namespace epsql::db::table
