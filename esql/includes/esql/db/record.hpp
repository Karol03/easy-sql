/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <inttypes.h>
#include <optional>
#include <string>
#include <vector>

#include "esql/utils/typeof.hpp"
#include "transaction.hpp"


namespace esql::db
{

template <typename Table>
class Record
{
public:
    Record(Transaction transaction)
        : m_transaction{std::move(transaction)}
        , m_current{std::nullopt}
        , m_previous{std::nullopt}
    {}

    Record(Transaction transaction, const Table& object)
        : m_transaction{std::move(transaction)}
        , m_current{object}
        , m_previous{object}
    {}

    Record(Transaction transaction, Table&& object)
        : m_transaction{std::move(transaction)}
        , m_current{object}
        , m_previous{std::move(object)}
    {}

    Record(const Record&) = delete;
    Record& operator=(const Record&) = delete;

    Record(Record&&) noexcept = default;
    Record& operator=(Record&&) noexcept = default;

    inline bool isExists() const { return m_current && *m_current->Id != 0; }
    inline operator bool() const { return isExists(); }

    inline Table& operator*() { return *m_current; }
    inline const Table& operator*() const { return *m_current; }
    inline Table* operator->() { return &(*m_current); }
    inline const Table* operator->() const { return &(*m_current); }

    inline void clean() { m_current = m_previous; }
    inline void create() { if (!m_current) m_current = Table{}; }
    inline void remove() { m_current.reset(); }

    inline void commit()
    {
        if (m_previous && !m_current)
        {
            m_transaction.commit(Delete{*m_previous});
        }
        else if (!m_previous && m_current)
        {
            m_transaction.commit(Create{*m_current});
        }
        else if (m_previous && m_current && (*m_previous) != (*m_current))
        {
            m_transaction.commit(Update{*m_current,
                                        m_previous->getValueAsString(m_previous->primaryKeyName()),
                                        changedFieldsList()});
        }
        m_previous = m_current;
    }

    inline Record<Table> clone()
    {
        if (!m_current)
            return Record<Table>(m_transaction.clone());
        else
            return Record<Table>(m_transaction.clone(), *m_current);
    }

private:
    std::vector<std::string> changedFieldsList()
    {
        const auto& fieldNames = m_current->fieldNames();
        auto result = std::vector<std::string>{};

        for (auto i = 0ull; i < fieldNames.size(); ++i)
        {
            const auto previousValue = m_previous->getValueAsString(fieldNames[i]);
            const auto currentValue = m_current->getValueAsString(fieldNames[i]);

            if (previousValue != currentValue)
            {
                result.push_back(fieldNames[i]);
            }
        }
        return result;
    }

private:
    Transaction m_transaction;
    std::optional<Table> m_current;
    std::optional<Table> m_previous;
};

}  // namespace esql::db
