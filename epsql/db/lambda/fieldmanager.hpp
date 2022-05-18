/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "semantic/operation.hpp"
#include "semantic/operationresult.hpp"
#include "semantic/operationtree.hpp"
#include "semantictree.hpp"


namespace epsql::db::lambda
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
        auto operation = semantic::makeOperation<T>(std::forward<Args>(args)...);
        return std::make_shared<semantic::OperationDataResult>(m_tree, std::move(operation));
    }

private:
    semantic::OperationTree m_tree;
};

}  // namespace epsql::db::lambda
