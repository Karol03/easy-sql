/**
 * Created by Karol Dudzic @ 2022
 */

#include "epsql/db/lambda/semantic/operationtree.hpp"

#include <algorithm>
#include <memory>
#include <sstream>
#include <stdexcept>



namespace epsql::db::lambda::semantic
{

void OperationTree::pushLeaf(std::shared_ptr<OperationResultBase> leaf)
{
    push(std::move(leaf));
}

void OperationTree::pushBranch(std::shared_ptr<OperationResultBase> branch,
                               std::shared_ptr<OperationResultBase> leftBranch,
                               std::shared_ptr<OperationResultBase> rightBranch)
{
    erase(std::move(leftBranch));
    erase(std::move(rightBranch));
    push(std::move(branch));
}

void OperationTree::walk(
        std::function<void(const OperationResultBase&)> walkFunction) const
{
    if (!m_branches.empty())
    {
        (*m_branches.begin())->walk(walkFunction);
    }
}

void OperationTree::validate()
{
    constexpr auto SINGLE_OPERATION_ROOT = 1ul;

    if (m_branches.size() != SINGLE_OPERATION_ROOT)
    {
        auto result = std::stringstream{};
        result << "Missing '&&' or '||' operator. Probably " \
                  "some Field(...) operator was used incorrectly or is abandoned.";
        throw std::runtime_error{result.str()};
    }
}

void OperationTree::push(std::shared_ptr<OperationResultBase> branch)
{
    m_branches.push_back(std::move(branch));
}

void OperationTree::erase(std::shared_ptr<OperationResultBase> branch)
{
    const auto isSameAddress = [&branch](const auto& b) { return &(*branch) == &(*b); };
    const auto it = std::find_if(m_branches.begin(), m_branches.end(), isSameAddress);
    if (it != m_branches.end())
        m_branches.erase(it);
}

}  // namespace epsql::db::lambda::semantic
