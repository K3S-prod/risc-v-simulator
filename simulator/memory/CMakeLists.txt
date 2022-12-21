
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/memory.cpp)
target_sources(simulator PRIVATE ${SOURCES})
target_include_directories(simulator PUBLIC ${PROJECT_BINARY_DIR}
                                         ${PROJECT_SOURCE_DIR}/third-party/ELFIO)
target_include_directories(sim_lib PUBLIC ${PROJECT_BINARY_DIR}
                                            ${PROJECT_SOURCE_DIR}/simulator/elf-loader)
