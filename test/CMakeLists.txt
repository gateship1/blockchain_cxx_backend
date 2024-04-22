#[=[ testing blockchain backend C++ engine #]=]

message(STATUS "added subdirectory ${CMAKE_CURRENT_LIST_DIR} to build...")

project(test-blockchain
    LANGUAGES CXX)

add_executable(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE -DUNITTEST)

if (ENABLE_LONG_TESTS)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE -DENABLE_LONG_TESTS)
endif (ENABLE_LONG_TESTS)

target_sources(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../src/sha256.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/../src
            ${doctest_SOURCE_DIR}/doctest)

set_target_properties(${PROJECT_NAME}
    PROPERTIES LINKER_LANGUAGE CXX
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
               TIMEOUT 300)
