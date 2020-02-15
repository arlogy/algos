cmake_minimum_required(VERSION 3.5)

project(word_dict LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_DEPS ../../deps)

set(HEADERS
    ${COMMON_DEPS}/path.hpp
    ${COMMON_DEPS}/timer.hpp
    deps/dfa_string_dict.h
    deps/dfa_tree.hpp
    deps/dfa_tree_utils.hpp
    src/word_dict.hpp
    src/main_utils.hpp
)

set(SOURCES
    deps/dfa_string_dict.cpp
    src/main.cpp
)

add_executable(word_dict ${HEADERS} ${SOURCES})
target_include_directories(word_dict PRIVATE ${COMMON_DEPS} deps src/dict)
