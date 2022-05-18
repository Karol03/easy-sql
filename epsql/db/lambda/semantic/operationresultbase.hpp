/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <memory>
#include <string>
#include <vector>


namespace epsql::db::lambda::semantic
{

class OperationTree;

struct OperationResultBase : public std::enable_shared_from_this<OperationResultBase>
{
public:
    OperationResultBase() = default;
    OperationResultBase(const OperationResultBase&) = delete;
    OperationResultBase& operator=(const OperationResultBase&) = delete;
    OperationResultBase(OperationResultBase&&) = delete;
    OperationResultBase& operator=(OperationResultBase&&) = delete;

    virtual ~OperationResultBase() = default;

    virtual uint32_t getDepth() const = 0;
    virtual std::string parse() const = 0;
    virtual std::vector<std::string> tables() const = 0;


    friend std::shared_ptr<OperationResultBase> operator&&(const std::shared_ptr<OperationResultBase>& lhs,
                                                           const std::shared_ptr<OperationResultBase>& rhs);
    friend std::shared_ptr<OperationResultBase> operator||(const std::shared_ptr<OperationResultBase>& lhs,
                                                           const std::shared_ptr<OperationResultBase>& rhs);

private:
    virtual OperationTree& tree() = 0;
};

}  // namespace epsql::db::lambda::semantic
