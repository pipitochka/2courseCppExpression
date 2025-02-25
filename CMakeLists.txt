cmake_minimum_required(VERSION 3.23)
project(Expression)

set(CMAKE_CXX_STANDARD 20)

include_directories(inc)

file(GLOB EXPRESSION_SOURCES
        src/BinaryExpression.cpp
        src/ConstantExpression.cpp
        src/MonoExpression.cpp
        src/VarExpression.cpp
)

file(GLOB TOKEN_SOURCES
        src/Token.cpp
)

file(GLOB PARSER_SOURCES
        src/Parser.cpp
)

add_library(EXPRESSION ${EXPRESSION_SOURCES})
add_library(TOKEN ${TOKEN_SOURCES})
add_library(PARSER ${PARSER_SOURCES})

add_executable(Expression main.cpp)

target_link_libraries(PARSER PRIVATE TOKEN EXPRESSION)
target_link_libraries(Expression PRIVATE EXPRESSION TOKEN PARSER)
