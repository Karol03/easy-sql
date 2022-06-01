/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "operationresultbase.hpp"


namespace esql::db::lambda::semantic
{

struct OperationTree
{
public:
    explicit OperationTree() = default;

    inline const OperationResultBase& main() const { return **m_branches.begin(); }

    void pushLeaf(std::shared_ptr<OperationResultBase> leaf);
    void pushBranch(std::shared_ptr<OperationResultBase> branch,
                    std::shared_ptr<OperationResultBase> leftBranch,
                    std::shared_ptr<OperationResultBase> rightBranch);
    void pushBranch(std::shared_ptr<OperationResultBase> leaf,
                    std::shared_ptr<OperationResultBase> branch);
    void validate();
    void walk(std::function<void(const OperationResultBase&)> walkFunction) const;
    std::vector<RelationChain> squash() const;

private:
    void push(std::shared_ptr<OperationResultBase> branch);
    void erase(std::shared_ptr<OperationResultBase> branch);
    std::shared_ptr<OperationResultBase> squash(std::shared_ptr<OperationResultBase> branch);

private:
    std::vector<std::shared_ptr<OperationResultBase>> m_branches;
};

}  // namespace esql::db::lambda::semantic
