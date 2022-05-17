cmake_minimum_required(VERSION 3.17)
project(MAPF)

set(CMAKE_CXX_STANDARD 14)

add_executable(MAPF AStarPlanner.cpp AStarPlanner.h CBS.cpp CBS.h MAPFInstance.cpp MAPFInstance.h PrioritizedSearch.cpp PrioritizedSearch.h task0/task0.cpp task1/task1.cpp task2/task2.cpp task3/task3.cpp Google_tests/test1.cpp)

add_subdirectory(Google_tests)