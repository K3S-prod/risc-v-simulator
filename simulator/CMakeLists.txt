
set(SOURCES main.cpp simulator.cpp)
add_executable(simulator ${SOURCES})
target_include_directories(simulator PUBLIC ${PROJECT_BINARY_DIR}
                                            ${PROJECT_SOURCE_DIR}/simulator/cpu ${PROJECT_SOURCE_DIR}/simulator/memory
                                            ${PROJECT_SOURCE_DIR}/simulator/elf-loader)


FILE(GLOB_RECURSE LIB_SOURCES *.cpp)
LIST(REMOVE_ITEM LIB_SOURCES ${PROJECT_SOURCE_DIR}/simulator/main.cpp)
add_library(sim_lib OBJECT ${LIB_SOURCES})


target_include_directories(sim_lib PUBLIC ${PROJECT_BINARY_DIR}
                                            ${PROJECT_SOURCE_DIR}/simulator/cpu ${PROJECT_SOURCE_DIR}/simulator/memory 
                                            ${PROJECT_SOURCE_DIR}/simulator/elf-loader)
target_include_directories(sim_lib PUBLIC ${PROJECT_BINARY_DIR}
                                            ${PROJECT_SOURCE_DIR}/third-party/ELFIO)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cpu)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/memory)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/elf-loader)
