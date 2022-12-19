
set(SOURCES cpu.cpp pipeline.cpp)
add_library(cpu SHARED ${SOURCES})
target_include_directories(cpu PUBLIC ${PROJECT_BINARY_DIR} 
                                           ${PROJECT_SOURCE_DIR}/include)
