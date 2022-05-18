/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "epsql/utils/typeof.hpp"
#include "lambda/field.hpp"
#include "lambda/fieldmanager.hpp"
#include "lambda/query.hpp"
#include "lambda/semantictree.hpp"


#define DEFINED_ARGS_WHERE(__QUERY_BODY, ...) \
    ::epsql::db::lambda::Query([__VA_ARGS__]() -> ::epsql::db::lambda::SemanticTree \
    { \
        auto __manager = ::epsql::db::lambda::FieldManager{}; \
        { [&]() -> std::shared_ptr<::epsql::db::lambda::semantic::OperationResultBase> __QUERY_BODY (); } \
        return __manager.result(); \
    })

#define UNWRAP_WHERE_ARGS1(_v1, ...) _v1
#define UNWRAP_WHERE_ARGS2(_v1, ...) _v1, UNWRAP_WHERE_ARGS1(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS3(_v1, ...) _v1, UNWRAP_WHERE_ARGS2(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS4(_v1, ...) _v1, UNWRAP_WHERE_ARGS3(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS5(_v1, ...) _v1, UNWRAP_WHERE_ARGS4(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS6(_v1, ...) _v1, UNWRAP_WHERE_ARGS5(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS7(_v1, ...) _v1, UNWRAP_WHERE_ARGS6(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS8(_v1, ...) _v1, UNWRAP_WHERE_ARGS7(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS9(_v1, ...) _v1, UNWRAP_WHERE_ARGS8(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS10(_v1, ...) _v1, UNWRAP_WHERE_ARGS9(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS11(_v1, ...) _v1, UNWRAP_WHERE_ARGS10(__VA_ARGS__)
#define UNWRAP_WHERE_ARGS12(_v1, ...) _v1, UNWRAP_WHERE_ARGS11(__VA_ARGS__)

#define WHERE0(__QUERY_BODY, ...) \
    ::epsql::db::lambda::Query([&]() -> ::epsql::db::lambda::SemanticTree \
    { \
        auto __manager = ::epsql::db::lambda::FieldManager{}; \
        { [&]() -> std::shared_ptr<::epsql::db::lambda::semantic::OperationResultBase> __QUERY_BODY (); } \
        return __manager.result(); \
    })
#define WHERE1(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS1(__VA_ARGS__))
#define WHERE2(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS2(__VA_ARGS__))
#define WHERE3(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS3(__VA_ARGS__))
#define WHERE4(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS4(__VA_ARGS__))
#define WHERE5(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS5(__VA_ARGS__))
#define WHERE6(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS6(__VA_ARGS__))
#define WHERE7(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS7(__VA_ARGS__))
#define WHERE8(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS8(__VA_ARGS__))
#define WHERE9(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS9(__VA_ARGS__))
#define WHERE10(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS10(__VA_ARGS__))
#define WHERE11(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS11(__VA_ARGS__))
#define WHERE12(__QUERY_BODY, ...) DEFINED_ARGS_WHERE(__QUERY_BODY, UNWRAP_WHERE_ARGS12(__VA_ARGS__))

#define WHERE_(__QUERY_BODY, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12, n, ...) \
    WHERE##n(__QUERY_BODY, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12)


#define where(...)   WHERE_(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1)

/**
 *  Class field accessor
 */
#define Field(__ClassField) ::epsql::db::lambda::Field(__manager, \
                                                       __ClassField##NameGetter(), \
                                                       ::epsql::utils::TypeOf<decltype(__ClassField)::ValueType>().type(), \
                                                       decltype(__ClassField)::name())
