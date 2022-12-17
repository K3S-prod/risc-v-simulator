
set(SOURCES test_elf_load.cpp)
add_executable(tests ${SOURCES})
target_include_directories(tests PUBLIC ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/third-party/googletest)
target_include_directories(tests PUBLIC ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/include)
