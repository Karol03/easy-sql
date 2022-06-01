/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace esql::db::lambda::semantic
{

struct OperationTree;


enum class EOperationResultType
{
    Data          = 0,
    And           = 1,
    Or            = 2,
    Contant_true  = 3,
    Contant_false = 4
};


struct RelationChain
{
    std::string expression;
    std::vector<std::pair<std::string, std::string>> connections;
};


struct OperationResultBase : public std::enable_shared_from_this<OperationResultBase>
{
public:
    OperationResultBase() = default;

    OperationResultBase(const OperationResultBase&) = delete;
    OperationResultBase& operator=(const OperationResultBase&) = delete;
    OperationResultBase(OperationResultBase&&) = delete;
    OperationResultBase& operator=(OperationResultBase&&) = delete;

    virtual ~OperationResultBase() = default;

    virtual EOperationResultType type() const = 0;
    virtual std::string parse() const = 0;
    virtual std::vector<std::string> tables() const = 0;
    virtual uint32_t getDepth() const = 0;
    virtual void walk(const std::function<void(const OperationResultBase&)>& walkFunction) const = 0;
    virtual std::vector<RelationChain> squash() const = 0;

    friend std::shared_ptr<OperationResultBase> operator&&(const std::shared_ptr<OperationResultBase>& lhs,
                                                           const std::shared_ptr<OperationResultBase>& rhs);
    friend std::shared_ptr<OperationResultBase> operator||(const std::shared_ptr<OperationResultBase>& lhs,
                                                           const std::shared_ptr<OperationResultBase>& rhs);
    friend std::shared_ptr<OperationResultBase> operator&&(const bool& lhs,
                                                           const std::shared_ptr<OperationResultBase>& rhs);
    friend std::shared_ptr<OperationResultBase> operator&&(const std::shared_ptr<OperationResultBase>& lhs,
                                                           const bool& rhs);
    friend std::shared_ptr<OperationResultBase> operator||(const bool& lhs,
                                                           const std::shared_ptr<OperationResultBase>& rhs);
    friend std::shared_ptr<OperationResultBase> operator||(const std::shared_ptr<OperationResultBase>& lhs,
                                                           const bool& rhs);

private:
    virtual OperationTree& tree() = 0;
};

}  // namespace esql::db::lambda::semantic
