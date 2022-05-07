/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "db/value/ivalue.hpp"
#include "utils/hash.hpp"


namespace epsql::db::table
{

class ReflectionRegister
{
public:
    virtual ~ReflectionRegister() = default;

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
    template <typename... FieldNames>
    inline static void assignAll(FieldNames&&... fieldFullNames)
    {
        (assign(fieldFullNames), ...);
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
};


template <typename T>
class ReflectionGroup : public ReflectionRegister
{
public:
    inline const char* typeOf(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it == m_names.end())
           return "";

        const auto itPos = it - m_names.begin();
        return m_types[itPos];
    }

    template <typename U>
    inline U* get(const char* name)
    {
        auto it = std::find_if(m_names.begin(), m_names.end(),
                               [&name](const auto* n) { return strcmp(name, n) == 0; });
        if (it == m_names.end())
            return nullptr;

        const auto itPos = it - m_names.begin();
        return reinterpret_cast<value::ValueT<U>*>(m_position[itPos])->nonEmptyPtr();
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

    inline static const auto& memberNames() { return m_names; }

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

protected:
    static inline bool isReferenced = {false};

private:
    std::vector<void*> m_position = {};
    std::vector<std::size_t> m_type = {};

    static inline std::vector<const char*> m_names = {};
    static inline std::vector<const char*> m_types = {};
};

}  // namespace epsql::db::table
