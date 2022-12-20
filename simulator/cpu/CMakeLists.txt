
set(SOURCES cpu.cpp pipeline.cpp instruction.cpp)
add_library(cpu STATIC ${SOURCES})
target_include_directories(cpu PUBLIC ${PROJECT_BINARY_DIR} 
                                           ${PROJECT_SOURCE_DIR}/include)
