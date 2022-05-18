/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <stdexcept>

#include "operationresultbase.hpp"
#include "operationtree.hpp"


namespace epsql::db::lambda::semantic
{

class Operation;
struct OperationTree;


struct OperationDataResult : public OperationResultBase
{
public:
    explicit OperationDataResult(OperationTree& tree,
                                 std::shared_ptr<Operation> operation);
    EOperationResultType type() const override;
    std::string parse() const override;
    std::vector<std::string> tables() const override;
    uint32_t getDepth() const override;
    void walk(const std::function<void(const OperationResultBase&)>& walkFunction) const override;

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
                                std::shared_ptr<OperationResultBase> rhs);
    EOperationResultType type() const override;
    std::string parse() const override;
    std::vector<std::string> tables() const override;
    uint32_t getDepth() const override;
    void walk(const std::function<void(const OperationResultBase&)>& walkFunction) const override;

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
                               std::shared_ptr<OperationResultBase> rhs);
    EOperationResultType type() const override;
    std::string parse() const override;
    std::vector<std::string> tables() const override;
    uint32_t getDepth() const override;
    void walk(const std::function<void(const OperationResultBase&)>& walkFunction) const override;

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
    auto result = std::make_shared<OperationAndResult>(lhs->tree(), depth, lhs, rhs);
    lhs->tree().pushBranch(result->shared_from_this(), lhs, rhs);
    return result;
}

inline std::shared_ptr<OperationResultBase> operator||(const std::shared_ptr<OperationResultBase>& lhs,
                                                       const std::shared_ptr<OperationResultBase>& rhs)
{
    if (!lhs)
        throw std::runtime_error{"Missing left side operation for 'OR' comparation!"};
    if (!rhs)
        throw std::runtime_error{"Missing right side operation for 'OR' comparation!"};

    const auto depth = std::max(lhs->getDepth(), rhs->getDepth()) + 1;
    auto result = std::make_shared<OperationOrResult>(lhs->tree(), depth, lhs, rhs);
    lhs->tree().pushBranch(result->shared_from_this(), lhs, rhs);
    return result;
}

}  // namespace epsql::db::lambda::semantic
