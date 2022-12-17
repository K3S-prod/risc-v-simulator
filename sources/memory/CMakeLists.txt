
set(SOURCES sim_memory.cpp)
add_library(memory SHARED ${SOURCES})
target_include_directories(memory PUBLIC ${PROJECT_BINARY_DIR} 
                                           ${PROJECT_SOURCE_DIR}/include)
target_include_directories(memory PUBLIC ${PROJECT_BINARY_DIR}
                                         ${PROJECT_SOURCE_DIR}/third-party/ELFIO)
