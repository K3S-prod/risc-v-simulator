set(SOURCES main.cpp simulator.cpp)
add_executable(simulator ${SOURCES})
target_include_directories(simulator PUBLIC ${PROJECT_BINARY_DIR}
                                            ${PROJECT_SOURCE_DIR}/simulator/cpu ${PROJECT_SOURCE_DIR}/simulator/memory)
target_link_libraries(simulator PUBLIC cpu memory)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cpu)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/memory)
