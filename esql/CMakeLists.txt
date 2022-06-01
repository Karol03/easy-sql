cmake_minimum_required(VERSION 3.5)

project(esql-lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_library(esql-lib
    STATIC
        includes/esql/errorstream.hpp
        includes/esql/sql.hpp

        includes/esql/db/database.hpp
        includes/esql/db/dbresult.hpp
        includes/esql/db/lambda.hpp
        includes/esql/db/core.hpp
        includes/esql/db/record.hpp
        includes/esql/db/transaction.hpp

        includes/esql/db/lambda/field.hpp
        includes/esql/db/lambda/fieldmanager.hpp
        includes/esql/db/lambda/query.hpp
        includes/esql/db/lambda/semantictree.hpp

        includes/esql/db/lambda/semantic/operation.hpp
        includes/esql/db/lambda/semantic/operationresult.hpp
        includes/esql/db/lambda/semantic/operationresultbase.hpp
        includes/esql/db/lambda/semantic/operationtree.hpp

        includes/esql/db/table/reflectiongroup.hpp
        includes/esql/db/table/table.hpp
        includes/esql/db/table/types.hpp

        includes/esql/db/query/createtable.hpp
        includes/esql/db/query/isexiststable.hpp
        includes/esql/db/query/queries.hpp
        includes/esql/db/query/query.hpp
        includes/esql/db/query/removetable.hpp
        includes/esql/db/query/sizetable.hpp
        includes/esql/db/query/translator/createrecord.hpp
        includes/esql/db/query/translator/removerecord.hpp
        includes/esql/db/query/translator/updaterecord.hpp

        includes/esql/db/value/default.hpp
        includes/esql/db/value/foreignkey.hpp
        includes/esql/db/value/ivalue.hpp
        includes/esql/db/value/value.hpp

        includes/esql/utils/hash.hpp
        includes/esql/utils/typeof.hpp

        src/db/table/reflectionregister.cpp
        src/db/table/foreignkeyreflectiongroup.cpp

        src/db/postgres_core.cpp

        src/db/lambda/field.cpp
        src/db/lambda/query.cpp
        src/db/lambda/semantictree.cpp

        src/db/lambda/semantic/operation.cpp
        src/db/lambda/semantic/operationresult.cpp
        src/db/lambda/semantic/operationtree.cpp
)
