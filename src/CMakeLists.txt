#[=[ blockchain backend C++ engine #]=]

message(STATUS "added subdirectory ${CMAKE_CURRENT_LIST_DIR} to build...")

project(blockchain-engine
    LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED)

set(${PROJECT_NAME}_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/block.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/blockchain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sha256.cpp)

target_sources(${PROJECT_NAME}
    PRIVATE ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
           ${doctest_SOURCE_DIR}/doctest)

set_target_properties(${PROJECT_NAME}
    PROPERTIES LINKER_LANGUAGE CXX
               ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

#[=[ blockchain server using PyBind11 with backend C++ engine #]=]
if (SERVER)

    include(${CMAKE_SOURCE_DIR}/cmake/pybind11_helper.cmake)

    project(backend
        LANGUAGES CXX)

    pybind11_add_module(${PROJECT_NAME}
        SHARED ${${PROJECT_NAME}_SOURCES}
               ${CMAKE_CURRENT_SOURCE_DIR}/bindings.cpp)

    target_include_directories(${PROJECT_NAME}
        PUBLIC ${pybind11_SOURCE_DIR}/include
               $ENV{PYPATH})

    target_link_libraries(${PROJECT_NAME}
        PRIVATE blockchain-engine)

    set_target_properties(${PROJECT_NAME}
        PROPERTIES LINKER_LANGUAGE CXX
                   ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

endif (SERVER)

#[=[ simulate a blockchain with backend C++ engine #]=]
if (SIMULATE)

    message(STATUS "added subdirectory ${CMAKE_CURRENT_LIST_DIR} to build...")

    project(simulate_blockchain
        LANGUAGES CXX)

    add_executable(${PROJECT_NAME})

    target_sources(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/simulate.cpp)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE blockchain-engine)

    set_target_properties(${PROJECT_NAME}
        PROPERTIES LINKER_LANGUAGE CXX
                   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

endif (SIMULATE)
