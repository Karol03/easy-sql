cmake_minimum_required(VERSION 3.5)

project(easy-sql LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/esql/includes)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/esql)

add_executable(easy-sql
    main.cpp
)

target_link_directories(easy-sql PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/esql)
target_link_libraries(easy-sql esql-lib)
