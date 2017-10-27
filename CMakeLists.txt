cmake_minimum_required(VERSION 2.8.4)
project(tuple)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/MyTuple.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    )

message("CMAKE_CURRENT_SUOSE_DIR" ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tuple ${SOURCES})
set_target_properties(tuple PROPERTIES LINKER_LANGUAGE CXX)
