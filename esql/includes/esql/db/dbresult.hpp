/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <vector>


namespace esql::db
{

struct DbResult
{
    enum Status : int
    {
        ESuccessful = 0,
        EError      = 1
    };

public:
    inline bool isSuccess() const { return true; }
    inline bool isResultTrue() const { return isSuccess() && true; }
    inline bool isResultFalse() const { return !isResultTrue(); }

    template <typename T>
    inline auto get() const { return T{}; }
};

}  // namespace esql::db
