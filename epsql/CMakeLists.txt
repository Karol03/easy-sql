cmake_minimum_required(VERSION 3.5)

project(epsql-lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_library(epsql-lib
    STATIC
        includes/epsql/errorstream.hpp
        includes/epsql/postgresql.hpp

        includes/epsql/db/database.hpp
        includes/epsql/db/dbresult.hpp
        includes/epsql/db/lambda.hpp
        includes/epsql/db/postgres.hpp
        includes/epsql/db/record.hpp
        includes/epsql/db/transaction.hpp

        includes/epsql/db/lambda/field.hpp
        includes/epsql/db/lambda/fieldmanager.hpp
        includes/epsql/db/lambda/query.hpp
        includes/epsql/db/lambda/semantictree.hpp

        includes/epsql/db/lambda/semantic/operation.hpp
        includes/epsql/db/lambda/semantic/operationresult.hpp
        includes/epsql/db/lambda/semantic/operationresultbase.hpp
        includes/epsql/db/lambda/semantic/operationtree.hpp

        includes/epsql/db/table/reflectiongroup.hpp
        includes/epsql/db/table/table.hpp
        includes/epsql/db/table/types.hpp

        includes/epsql/db/query/createtable.hpp
        includes/epsql/db/query/isexiststable.hpp
        includes/epsql/db/query/queries.hpp
        includes/epsql/db/query/query.hpp
        includes/epsql/db/query/removetable.hpp
        includes/epsql/db/query/sizetable.hpp
        includes/epsql/db/query/translator/createrecord.hpp
        includes/epsql/db/query/translator/removerecord.hpp
        includes/epsql/db/query/translator/updaterecord.hpp

        includes/epsql/db/value/default.hpp
        includes/epsql/db/value/foreignkey.hpp
        includes/epsql/db/value/ivalue.hpp
        includes/epsql/db/value/value.hpp

        includes/epsql/utils/hash.hpp
        includes/epsql/utils/typeof.hpp


        src/db/lambda/field.cpp
        src/db/lambda/query.cpp
        src/db/lambda/semantictree.cpp

        src/db/lambda/semantic/operation.cpp
        src/db/lambda/semantic/operationresult.cpp
        src/db/lambda/semantic/operationtree.cpp
)
