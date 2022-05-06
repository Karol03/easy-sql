cmake_minimum_required(VERSION 3.5)

project(easy-sql LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/epsql)

add_executable(easy-sql
    main.cpp

    epsql/postgresql.hpp

    epsql/db/database.hpp
    epsql/db/lambda.hpp
    epsql/db/query.hpp
    epsql/db/record.hpp
    epsql/db/table.hpp
    epsql/db/transaction.hpp
    epsql/db/types.hpp
    epsql/db/value.hpp

    epsql/db/lambda/field.hpp
    epsql/db/lambda/operation.hpp
    epsql/db/lambda/query.hpp

    epsql/utils/hash.hpp
    epsql/utils/stringcompare.hpp
    epsql/utils/typeof.hpp
)
