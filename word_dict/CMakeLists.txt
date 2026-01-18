cmake_minimum_required(VERSION 3.5)

project(word_dict LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    src/common/path.hpp
    src/common/timer.hpp
    src/lookup/dfa_string_dict.h
    src/lookup/dfa_tree.hpp
    src/lookup/dfa_tree_utils.hpp
    src/lookup/word_dict.hpp
    src/main_utils.hpp
)

set(SOURCES
    src/lookup/dfa_string_dict.cpp
    src/main.cpp
)

add_executable(word_dict ${HEADERS} ${SOURCES})
target_include_directories(word_dict PRIVATE src/common src/lookup)
