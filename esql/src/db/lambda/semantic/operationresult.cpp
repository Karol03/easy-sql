/**
 * Created by Karol Dudzic @ 2022
 */

#include "esql/db/lambda/semantic/operationresult.hpp"

#include <unordered_set>
#include <sstream>

#include "esql/db/lambda/semantic/operation.hpp"


namespace esql::db::lambda::semantic
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

std::vector<RelationChain> OperationDataResult::squash() const
{
    const auto table = tables();
    if (table.size() == 1ul)
        return {{parse(), {{table.front(), std::string{}}}}};
    else if (table.size() == 2ul)
        return {{parse(), {{table.front(), *(table.begin() + 1)}}}};
    else
        throw std::logic_error{"An error during tree sqush appears. Data operation has no associated table!"};
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

std::vector<RelationChain> OperationAndResult::squash() const
{
    auto result = std::vector<RelationChain>{};
    const auto lhs = m_lhs->squash();
    const auto rhs = m_rhs->squash();

    for (const auto& lhsVec : lhs)
    {
        for (const auto& rhsVec : rhs)
        {
            auto expression = lhsVec.expression + " AND " + rhsVec.expression;
            auto tables = lhsVec.connections;
            tables.insert(tables.end(), rhsVec.connections.begin(), rhsVec.connections.end());
            result.push_back({std::move(expression), std::move(tables)});
        }
    }

    return result;
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

std::vector<RelationChain> OperationOrResult::squash() const
{
    auto lhs = m_lhs->squash();
    auto rhs = m_rhs->squash();
    lhs.insert(lhs.end(), std::make_move_iterator(rhs.begin()),
                          std::make_move_iterator(rhs.end()));
    return lhs;
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

// ----------------------------------------------------

OperationFalseResult::OperationFalseResult(OperationTree& tree,
                                           uint32_t depth)
    : m_tree{tree}
    , m_depth{depth}
{}

EOperationResultType OperationFalseResult::type() const
{
    return EOperationResultType::Contant_false;
}

std::string OperationFalseResult::parse() const
{
    return {};
}

std::vector<RelationChain> OperationFalseResult::squash() const
{
    return {};
}

std::vector<std::string> OperationFalseResult::tables() const
{
    return {};
}

uint32_t OperationFalseResult::getDepth() const
{
    return m_depth;
}

void OperationFalseResult::walk(
        const std::function<void(const OperationResultBase&)>& walkFunction) const
{
    walkFunction(*this);
}

// ----------------------------------------------------

OperationTrueResult::OperationTrueResult(OperationTree& tree,
                                         uint32_t depth)
    : m_tree{tree}
    , m_depth{depth}
{}

EOperationResultType OperationTrueResult::type() const
{
    return EOperationResultType::Contant_true;
}

std::string OperationTrueResult::parse() const
{
    return {};
}

std::vector<RelationChain> OperationTrueResult::squash() const
{
    return {};
}

std::vector<std::string> OperationTrueResult::tables() const
{
    return {};
}

uint32_t OperationTrueResult::getDepth() const
{
    return m_depth;
}

void OperationTrueResult::walk(
        const std::function<void(const OperationResultBase&)>& walkFunction) const
{
    walkFunction(*this);
}

}  // namespace esql::db::lambda::semantic
