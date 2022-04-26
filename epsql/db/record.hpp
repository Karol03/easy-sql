/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <inttypes.h>
#include <optional>

#include "transaction.hpp"
#include "utils/hash.hpp"


namespace epsql::db
{

template <typename Table>
class Record
{
public:
    Record(Transaction transaction)
        : m_object{std::nullopt}
        , m_hash{0}
        , m_uuid{0}
        , m_transaction{std::move(transaction)}
    {}

    Record(Transaction transaction, Table&& object)
        : m_object{std::move(object)}
        , m_hash{utils::Hash::hash(&(*m_object))}
        , m_uuid{*m_object->Id}
        , m_transaction{std::move(transaction)}
    {}

    Record(const Record&) = delete;
    Record& operator=(const Record&) = delete;

    Record(Record&&) noexcept = default;
    Record& operator=(Record&&) noexcept = default;

    inline bool isExists() const { return m_object && *m_object->Id != 0; }
    inline operator bool() const { return isExists(); }

    inline Table& operator*() { return *m_object; }
    inline const Table& operator*() const { return *m_object; }
    inline Table* operator->() { return &(*m_object); }
    inline const Table* operator->() const { return &(*m_object); }

    inline void create() { m_object = Table{}; }
    inline void remove() { m_object.reset(); }
    inline void commit()
    {
        if (m_hash != 0 && !m_object)
        {
            m_transaction.commit(Delete{m_uuid});
            m_hash = 0;
            return;
        }

        auto newHash = utils::Hash::hash(&(*m_object));
        if (m_hash == 0 && m_object)
        {
            m_transaction.commit(Create{*m_object});
            m_hash = newHash;
        }
        else if (m_hash != 0 && m_object && m_hash != newHash)
        {
            m_transaction.commit(Update{*m_object});
            m_hash = newHash;
        }
    }

    inline Record<Table> clone()
    {
        if (!m_object)
            return Record<Table>(m_transaction.clone());
        else
            return Record<Table>(m_transaction.clone(), *m_object);
    }

private:
    std::optional<Table> m_object;
    uint64_t m_hash;
    uint64_t m_uuid;
    Transaction m_transaction;
};

}  // namespace epsql::db
