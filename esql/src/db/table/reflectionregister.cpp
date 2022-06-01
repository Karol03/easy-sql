/**
 * Created by Karol Dudzic @ 2022
 */

#include "esql/db/table/reflectiongroup.hpp"


namespace esql::db::table
{

std::optional<std::string> ReflectionRegister::tableName(uint64_t uuid)
{
    if (m_tableReferenceUuids.find(uuid) != m_tableReferenceUuids.end())
        return m_tableReferenceUuids[uuid];
    return std::nullopt;
}

std::optional<uint32_t> ReflectionRegister::reference(const std::string& fieldFullName)
{
    if (m_referenceHashes.find(fieldFullName) != m_referenceHashes.end())
        return m_referenceHashes[fieldFullName];
    return std::nullopt;
}

std::optional<std::string> ReflectionRegister::reference(const uint32_t& fieldUuid)
{
    if (m_referenceUuids.find(fieldUuid) != m_referenceUuids.end())
        return m_referenceUuids[fieldUuid];
    return std::nullopt;
}

void ReflectionRegister::assign(const char* fieldFullName)
{
    const auto uuid = utils::Hash::hash(fieldFullName);
    if (m_referenceHashes.find(fieldFullName) == m_referenceHashes.end())
    {
        checkUuidUniqueness(fieldFullName, uuid);
        insertFieldReference(fieldFullName, uuid);
    }
}

void ReflectionRegister::checkUuidUniqueness(const char* fieldFullName, const uint32_t uuid)
{
    if (m_referenceUuids.find(uuid) != m_referenceUuids.end())
    {
        const auto result = std::string{"Cannot create field '"} + fieldFullName +
                            "' the same hash existed for '" +
                            m_referenceUuids[uuid] + "'";
        throw std::logic_error{result.c_str()};
    }
}

void ReflectionRegister::insertFieldReference(const char* fieldFullName, const uint32_t uuid)
{
    m_referenceHashes[fieldFullName] = uuid;
    m_referenceUuids[uuid] = fieldFullName;
}

}  // namespace esql::db::table
