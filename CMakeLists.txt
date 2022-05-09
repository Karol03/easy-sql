cmake_minimum_required(VERSION 3.5)

project(easy-sql LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/epsql)

add_executable(easy-sql
    main.cpp

    epsql/errorstream.hpp
    epsql/postgresql.hpp

    epsql/db/database.hpp
    epsql/db/dbresult.hpp
    epsql/db/lambda.hpp
    epsql/db/postgres.hpp
    epsql/db/record.hpp
    epsql/db/transaction.hpp

    epsql/db/lambda/field.hpp
    epsql/db/lambda/operation.hpp
    epsql/db/lambda/query.hpp

    epsql/db/table/reflectiongroup.hpp
    epsql/db/table/table.hpp
    epsql/db/table/types.hpp

    epsql/db/query/createtable.hpp
    epsql/db/query/isexiststable.hpp
    epsql/db/query/queries.hpp
    epsql/db/query/query.hpp
    epsql/db/query/removetable.hpp
    epsql/db/query/sizetable.hpp
    epsql/db/query/translator/createrecord.hpp
    epsql/db/query/translator/removerecord.hpp
    epsql/db/query/translator/updaterecord.hpp

    epsql/db/value/foreignkey.hpp
    epsql/db/value/ivalue.hpp
    epsql/db/value/value.hpp

    epsql/utils/hash.hpp
    epsql/utils/typeof.hpp
)
