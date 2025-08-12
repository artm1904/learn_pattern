cmake_minimum_required(VERSION 3.10)
project(lr_test VERSION 0.1.0)

enable_testing()

add_executable(${PROJECT_NAME}
    source/01_strategy/duckTests.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/01_strategy/SimUDuck
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE

    GTest::gmock GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
