/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "esql/db/value/ivalue.hpp"
#include "esql/utils/hash.hpp"
#include "types.hpp"


namespace esql::db::table
{

class ReflectionRegister
{
public:
    virtual ~ReflectionRegister() = default;

    static std::optional<std::string> tableName(uint64_t uuid);
    static std::optional<uint32_t> reference(const std::string& fieldFullName);
    static std::optional<std::string> reference(const uint32_t& fieldUuid);

protected:
    template <typename ClassUuid, typename ClassName, typename... FieldNames>
    inline static void assignAll(ClassUuid uuid, ClassName className,
                                 FieldNames&&... fieldFullNames)
    {
        (assign(fieldFullNames), ...);
        m_tableReferenceUuids[uuid] = className;
    }

    static void assign(const char* fieldFullName);
    static void checkUuidUniqueness(const char* fieldFullName, const uint32_t uuid);
    static void insertFieldReference(const char* fieldFullName, const uint32_t uuid);

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
        void insert(std::string tableName,
                    std::string fieldName,
                    std::string referenceTable,
                    std::string referenceField);
        std::vector<std::pair<std::string, std::string>> pathStraight(
                std::string beginTableName,
                std::string endTableName);
        std::vector<std::pair<std::string, std::string>> pathMultiple(
                std::string beginTableName,
                std::vector<std::string> possibleEndTables);
        std::vector<std::pair<std::string, std::string>> pathFinding(
                const std::string& beginTableName,
                const std::vector<std::string>& possibleEndTables);

    private:
        std::vector<std::pair<std::string, std::string>> composeResult(
                std::unordered_map<std::string, std::string> path,
                std::string endTableName);
        std::string relationFor(
                const std::string& tableName,
                const std::string& endTableName,
                const FieldHintsType& fieldHints);
        bool silentValidateBranchesExist(const std::string& beginName, const std::string& endName) const;
        void validateBranchesExist(const std::string& beginName, const std::string& endName) const;

    private:
        TableRelations m_relations;
    };

public:
    virtual ~ForeignKeyReflectionGroup() = default;

    static inline std::vector<std::pair<std::string, std::string>> findTablesRelationPath(
            std::string beginTableName,
            std::string endTableName)
    {
        if (beginTableName == endTableName)
            return {};
        return m_tree.pathStraight(std::move(beginTableName), std::move(endTableName));
    }

    static inline std::vector<std::pair<std::string, std::string>> findTablesRelationPath(
            std::string beginTableName,
            std::vector<std::string> possibleEndTables)
    {
        return m_tree.pathMultiple(std::move(beginTableName), std::move(possibleEndTables));
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
        if (auto* result = retrive<Point>(type, name))
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

}  // namespace esql::db::table
