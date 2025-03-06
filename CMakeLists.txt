cmake_minimum_required(VERSION 3.23)
project(Expression)

set(CMAKE_CXX_STANDARD 20)

file(GLOB TOKEN_SOURCES
        src/Token.cpp
)

add_library(TOKEN ${TOKEN_SOURCES})

add_executable(differentiator main.cpp)

target_link_libraries(differentiator PRIVATE TOKEN)

file(GLOB TEST_SOURCES
        tests/testDiff.cpp
        tests/testInput.cpp
        tests/testEval.cpp
)

foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE) 
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PRIVATE TOKEN)
    set_target_properties(${test_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests 
    )
endforeach()