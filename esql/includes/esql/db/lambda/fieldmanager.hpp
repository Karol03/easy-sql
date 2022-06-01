/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "semantic/operation.hpp"
#include "semantic/operationresult.hpp"
#include "semantic/operationtree.hpp"
#include "semantictree.hpp"


namespace esql::db::lambda
{

struct FieldManager
{
public:
    using OperationResult = std::shared_ptr<semantic::OperationResultBase>;

public:
    inline SemanticTree result()
    {
        return SemanticTree{std::move(m_tree)};
    }

    template <typename T, typename... Args>
    inline OperationResult operation(Args&&... args)
    {
        using namespace semantic;
        auto operation = makeOperation<T>(std::forward<Args>(args)...);
        auto result = std::make_shared<OperationDataResult>(m_tree, std::move(operation));
        m_tree.pushLeaf(result->shared_from_this());
        return result;
    }

private:
    semantic::OperationTree m_tree;
};

}  // namespace esql::db::lambda
