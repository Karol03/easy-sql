/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <set>
#include <sstream>

#include "operation.hpp"
#include "operationresultbase.hpp"
#include "operationtree.hpp"


namespace epsql::db::lambda::semantic
{

struct OperationDataResult : public OperationResultBase
{
public:
    explicit OperationDataResult(OperationTree& tree,
                                 std::shared_ptr<Operation> operation)
        : m_tree{tree}
        , m_operation{std::move(operation)}
    {
        m_tree.pushLeaf(shared_from_this());
    }

    inline std::string parse() const override
    {
        return m_operation->parse();
    }

    inline std::vector<std::string> tables() const override
    {
        return m_operation->tables();
    }

    inline uint32_t getDepth() const override
    {
        return 0;
    }

private:
    inline OperationTree& tree() override { return m_tree; }

private:
    OperationTree& m_tree;
    std::shared_ptr<Operation> m_operation;
};


struct OperationAndResult : public OperationResultBase
{
public:
    explicit OperationAndResult(OperationTree& tree,
                                uint32_t depth,
                                std::shared_ptr<OperationResultBase> lhs,
                                std::shared_ptr<OperationResultBase> rhs)
        : m_tree{tree}
        , m_depth{depth}
        , m_lhs{std::move(lhs)}
        , m_rhs{std::move(rhs)}
    {
        m_tree.pushBranch(shared_from_this(), m_lhs, m_rhs);
    }

    inline std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_lhs->parse() << " AND " << m_rhs->parse() << ")";
        return result.str();
    }

    inline std::vector<std::string> tables() const override
    {
        auto lhsResult = m_lhs->tables();
        auto rhsResult = m_lhs->tables();
        auto result = std::set<std::string>{};
        result.insert(lhsResult.begin(), lhsResult.end());
        result.insert(rhsResult.begin(), rhsResult.end());
        return std::vector(result.begin(), result.end());
    }

    inline uint32_t getDepth() const override
    {
        return m_depth;
    }

private:
    inline OperationTree& tree() override { return m_tree; }

private:
    OperationTree& m_tree;
    uint32_t m_depth;
    std::shared_ptr<OperationResultBase> m_lhs;
    std::shared_ptr<OperationResultBase> m_rhs;
};


struct OperationOrResult : public OperationResultBase
{
public:
    explicit OperationOrResult(OperationTree& tree,
                               uint32_t depth,
                               std::shared_ptr<OperationResultBase> lhs,
                               std::shared_ptr<OperationResultBase> rhs)
        : m_tree{tree}
        , m_depth{depth}
        , m_lhs{std::move(lhs)}
        , m_rhs{std::move(rhs)}
    {
        m_tree.pushBranch(shared_from_this(), m_lhs, m_rhs);
    }

    inline std::string parse() const override
    {
        auto result = std::stringstream{};
        result << "(" << m_lhs->parse() << " OR " << m_rhs->parse() << ")";
        return result.str();
    }

    inline std::vector<std::string> tables() const override
    {
        auto lhsResult = m_lhs->tables();
        auto rhsResult = m_lhs->tables();
        auto result = std::set<std::string>{};
        result.insert(lhsResult.begin(), lhsResult.end());
        result.insert(rhsResult.begin(), rhsResult.end());
        return std::vector(result.begin(), result.end());
    }

    inline uint32_t getDepth() const override
    {
        return m_depth;
    }

private:
    inline OperationTree& tree() override { return m_tree; }

private:
    OperationTree& m_tree;
    uint32_t m_depth;
    std::shared_ptr<OperationResultBase> m_lhs;
    std::shared_ptr<OperationResultBase> m_rhs;
};


inline std::shared_ptr<OperationResultBase> operator&&(const std::shared_ptr<OperationResultBase>& lhs,
                                                       const std::shared_ptr<OperationResultBase>& rhs)
{
    if (!lhs)
        throw std::runtime_error{"Missing left side operation for 'AND' comparation!"};
    if (!rhs)
        throw std::runtime_error{"Missing right side operation for 'AND' comparation!"};

    const auto depth = std::max(lhs->getDepth(), rhs->getDepth()) + 1;
    return std::make_shared<OperationAndResult>(lhs->tree(), depth, lhs, rhs);
}

inline std::shared_ptr<OperationResultBase> operator||(const std::shared_ptr<OperationResultBase>& lhs,
                                                       const std::shared_ptr<OperationResultBase>& rhs)
{
    if (!lhs)
        throw std::runtime_error{"Missing left side operation for 'OR' comparation!"};
    if (!rhs)
        throw std::runtime_error{"Missing right side operation for 'OR' comparation!"};

    const auto depth = std::max(lhs->getDepth(), rhs->getDepth()) + 1;
    return std::make_shared<OperationOrResult>(lhs->tree(), depth, lhs, rhs);
}

template <typename T, typename... Args>
inline auto makeOperationResult(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace epsql::db::lambda::semantic
