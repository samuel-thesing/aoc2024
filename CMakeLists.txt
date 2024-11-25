cmake_minimum_required(VERSION 3.29)
project(aoc2015)

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_DEPENDENCY_DIR "${CMAKE_SOURCE_DIR}/deps")
set(PROJECT_ARTEFACT_DIR  "${CMAKE_SOURCE_DIR}/bin")
set(PROJECT_SRC_DIR  "${CMAKE_SOURCE_DIR}/src")
set(PROJECT_BINARY_DIR "${PROJECT_ARTEFACT_DIR}")

include("utils.cmake")

foreach(DAY RANGE 1 25)
    if (DAY LESS 10)
        set(DAY_NAME "0${DAY}")
    else()
        set(DAY_NAME "${DAY}")
    endif()
    set(DAY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/${DAY_NAME}")
    if(IS_DIRECTORY ${DAY_PATH})
        add_subdirectory("src/${DAY_NAME}")
    endif()
endforeach()




