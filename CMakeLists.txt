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

add_library(EXPRESSION ${EXPRESSION_SOURCES})

add_executable(Expression main.cpp
        src/ConstantExpression.cpp)

target_link_libraries(Expression PRIVATE EXPRESSION)