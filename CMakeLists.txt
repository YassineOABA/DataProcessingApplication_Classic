cmake_minimum_required(VERSION 3.16)

project(DataProcessingApplication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(data_processing_app
    app/main.cpp
    app/application.cpp
    core/models/field.cpp
    core/models/record.cpp
    core/models/dataSet.cpp
    core/services/transformer.cpp
)

target_include_directories(data_processing_app PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(data_processing_app PRIVATE nlohmann_json::nlohmann_json)

enable_testing()

add_executable(io_tests
    tests/test_io.cpp
    io/writer.cpp
    io/textParser.cpp
)

target_include_directories(io_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(io_tests PRIVATE GTest::gtest_main nlohmann_json::nlohmann_json)

add_executable(utils_tests
    tests/test_utils.cpp
    utils/config.cpp
)

target_include_directories(utils_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(utils_tests PRIVATE GTest::gtest_main)

add_executable(model_tests
    tests/test_models.cpp
    core/models/field.cpp
    core/models/record.cpp
    core/models/dataSet.cpp
)

target_include_directories(model_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(model_tests PRIVATE GTest::gtest_main)

add_test(NAME io_tests COMMAND io_tests)
add_test(NAME utils_tests COMMAND utils_tests)
add_test(NAME model_tests COMMAND model_tests)
