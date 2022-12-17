
set(SOURCES tests.cpp)
add_executable(tests ${SOURCES})
target_include_directories(tests PUBLIC ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/third-party/googletest)
target_include_directories(tests PUBLIC ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(tests PUBLIC cpu memory common)
target_link_libraries(tests PUBLIC GTest::gtest_main)
