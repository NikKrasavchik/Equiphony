cmake_minimum_required(VERSION 3.5)
project(cmake-example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -fopenmp")

set(SRC_LIST Levenshtein.cpp)
# set(SRC_LIST test.cpp)
# set (SRC_LIST t.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
