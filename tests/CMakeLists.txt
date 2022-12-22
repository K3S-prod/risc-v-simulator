cmake_minimum_required(VERSION 3.14)

set(SOURCES tests.cpp)
add_executable(tests ${SOURCES})
target_link_libraries(tests PUBLIC sim_lib)
target_include_directories(tests PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/simulator/elf-loader  
                                        ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/simulator/)


include(FetchContent)   
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

target_link_libraries(
    tests
    PUBLIC
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_custom_command(TARGET tests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/tests/ $<TARGET_FILE_DIR:tests>)
