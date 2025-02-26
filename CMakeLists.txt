cmake_minimum_required(VERSION 3.23)
project(Expression)

set(CMAKE_CXX_STANDARD 20)

include_directories(inc)

file(GLOB EXPRESSION_SOURCES
        src/BinaryExpression.cpp
        src/ConstantExpression.cpp
        src/MonoExpression.cpp
        src/VarExpression.cpp
        src/Optimization.cpp
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

add_executable(differentiator main.cpp)

target_link_libraries(PARSER PRIVATE TOKEN EXPRESSION)
target_link_libraries(differentiator PRIVATE EXPRESSION TOKEN PARSER)

file(GLOB TEST_SOURCES
        tests/testDiff.cpp
        tests/testInput.cpp
        tests/testEval.cpp
)

foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE) 
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PRIVATE EXPRESSION TOKEN PARSER)
    set_target_properties(${test_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests 
    )
endforeach()