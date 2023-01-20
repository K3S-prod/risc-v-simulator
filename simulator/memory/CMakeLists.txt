set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/memory.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/mmu.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/cache.cpp)

add_library(memory ${SOURCES})
