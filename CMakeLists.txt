cmake_minimum_required(VERSION 3.20.0)

project(blockchain
    LANGUAGES CXX)

include(cmake/internal_helpers.cmake)
include(cmake/prevent_insource_builds.cmake)
include(cmake/doctest_helper.cmake)

add_subdirectory(src)

if (UNITTEST)
    add_subdirectory(test)
endif (UNITTEST)

