/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "operationresultbase.hpp"


namespace epsql::db::lambda::semantic
{

struct OperationTree
{
public:
    explicit OperationTree() = default;

    void pushLeaf(std::shared_ptr<OperationResultBase> leaf)
    {
        push(std::move(leaf));
    }

    void pushBranch(std::shared_ptr<OperationResultBase> branch,
                    std::shared_ptr<OperationResultBase> leftBranch,
                    std::shared_ptr<OperationResultBase> rightBranch)
    {
        erase(std::move(leftBranch));
        erase(std::move(rightBranch));
        push(std::move(branch));
    }

    const OperationResultBase& main() const
    {
        return **m_branches.begin();
    }

    void walk(std::function<void(const OperationResultBase&)> walkFunction) const
    {
        if (!m_branches.empty())
        {
            (*m_branches.begin())->walk(walkFunction);
        }
    }

    void validate()
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

private:
    void push(std::shared_ptr<OperationResultBase> branch)
    {
        m_branches.push_back(std::move(branch));
    }

    void erase(std::shared_ptr<OperationResultBase> branch)
    {
        const auto isSameAddress = [&branch](const auto& b) { return &(*branch) == &(*b); };
        const auto it = std::find_if(m_branches.begin(), m_branches.end(), isSameAddress);
        if (it != m_branches.end())
            m_branches.erase(it);
    }

private:
    std::vector<std::shared_ptr<OperationResultBase>> m_branches;
};

}  // namespace epsql::db::lambda::semantic
