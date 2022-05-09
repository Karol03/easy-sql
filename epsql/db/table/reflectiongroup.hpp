/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>

#include "db/value/ivalue.hpp"
#include "types.hpp"
#include "utils/hash.hpp"


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


template <typename T>
class ReflectionGroup : public ReflectionRegister
{
public:
    inline uint64_t typeOf(const char* name) const
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

    inline std::variant<Text, Smallint, Int, Bigint, Float8, Real, Boolean, void*> var(
            const char* name) const
    {
        const auto type = typeOf(name);

        if (type == 0ull)
            return nullptr;
        else if (type == utils::TypeOf<Text>().type())
        {
            if (const auto* result = ptr<Text>(name))
                return *result;
        }
        else if (type == utils::TypeOf<Smallint>().type())
        {
            if (const auto* result = ptr<Smallint>(name))
                return *result;
        }
        else if (type == utils::TypeOf<Int>().type())
        {
            if (const auto* result = ptr<Int>(name))
                return *result;
        }
        else if (type == utils::TypeOf<Bigint>().type())
        {
            if (const auto* result = ptr<Bigint>(name))
                return *result;
        }
        else if (type == utils::TypeOf<Float8>().type())
        {
            if (const auto* result = ptr<Float8>(name))
                return *result;
        }
        else if (type == utils::TypeOf<Double>().type())
        {
            if (const auto* result = ptr<Double>(name))
                return *result;
        }
        else if (type == utils::TypeOf<Real>().type())
        {
            if (const auto* result = ptr<Real>(name))
                return *result;
        }
        else if (type == utils::TypeOf<Boolean>().type())
        {
            if (const auto* result = ptr<Boolean>(name))
                return *result;
        }
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

protected:
    inline static const auto& memberNames() { return m_names; }

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

protected:
    static inline bool isReferenced = {false};

private:
    std::vector<void*> m_position = {};
    std::vector<std::size_t> m_type = {};

    static inline std::vector<const char*> m_names = {};
    static inline std::vector<uint64_t> m_types = {};
};

}  // namespace epsql::db::table
