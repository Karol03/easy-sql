/**
 * Created by Karol Dudzic @ 2022
 */

#include "epsql/db/lambda/semantic/operationresult.hpp"

#include <unordered_set>
#include <sstream>

#include "epsql/db/lambda/semantic/operation.hpp"


namespace epsql::db::lambda::semantic
{

OperationDataResult::OperationDataResult(OperationTree& tree,
                                         std::shared_ptr<Operation> operation)
    : m_tree{tree}
    , m_operation{std::move(operation)}
{}

EOperationResultType OperationDataResult::type() const
{
    return EOperationResultType::Data;
}

std::string OperationDataResult::parse() const
{
    return m_operation->parse();
}

std::vector<std::string> OperationDataResult::tables() const
{
    return m_operation->tables();
}

uint32_t OperationDataResult::getDepth() const
{
    return 0;
}

void OperationDataResult::walk(
        const std::function<void(const OperationResultBase&)>& walkFunction) const
{
    walkFunction(*this);
}

// ----------------------------------------------------

OperationAndResult::OperationAndResult(OperationTree& tree,
                                       uint32_t depth,
                                       std::shared_ptr<OperationResultBase> lhs,
                                       std::shared_ptr<OperationResultBase> rhs)
    : m_tree{tree}
    , m_depth{depth}
    , m_lhs{std::move(lhs)}
    , m_rhs{std::move(rhs)}
{}

EOperationResultType OperationAndResult::type() const
{
    return EOperationResultType::And;
}

std::string OperationAndResult::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_lhs->parse() << " AND " << m_rhs->parse() << ")";
    return result.str();
}

std::vector<std::string> OperationAndResult::tables() const
{
    auto lhsResult = m_lhs->tables();
    auto rhsResult = m_lhs->tables();
    auto result = std::unordered_set<std::string>{};
    result.insert(lhsResult.begin(), lhsResult.end());
    result.insert(rhsResult.begin(), rhsResult.end());
    return std::vector(result.begin(), result.end());
}

uint32_t OperationAndResult::getDepth() const
{
    return m_depth;
}

void OperationAndResult::walk(
        const std::function<void(const OperationResultBase&)>& walkFunction) const
{
    walkFunction(*this);
    m_lhs->walk(walkFunction);
    m_rhs->walk(walkFunction);
}

// ----------------------------------------------------

OperationOrResult::OperationOrResult(OperationTree& tree,
                                     uint32_t depth,
                                     std::shared_ptr<OperationResultBase> lhs,
                                     std::shared_ptr<OperationResultBase> rhs)
    : m_tree{tree}
    , m_depth{depth}
    , m_lhs{std::move(lhs)}
    , m_rhs{std::move(rhs)}
{}

EOperationResultType OperationOrResult::type() const
{
    return EOperationResultType::Or;
}

std::string OperationOrResult::parse() const
{
    auto result = std::stringstream{};
    result << "(" << m_lhs->parse() << " OR " << m_rhs->parse() << ")";
    return result.str();
}

std::vector<std::string> OperationOrResult::tables() const
{
    auto lhsResult = m_lhs->tables();
    auto rhsResult = m_lhs->tables();
    auto result = std::unordered_set<std::string>{};
    result.insert(lhsResult.begin(), lhsResult.end());
    result.insert(rhsResult.begin(), rhsResult.end());
    return std::vector(result.begin(), result.end());
}

uint32_t OperationOrResult::getDepth() const
{
    return m_depth;
}

void OperationOrResult::walk(
        const std::function<void(const OperationResultBase&)>& walkFunction) const
{
    walkFunction(*this);
    m_lhs->walk(walkFunction);
    m_rhs->walk(walkFunction);
}

}  // namespace epsql::db::lambda::semantic
