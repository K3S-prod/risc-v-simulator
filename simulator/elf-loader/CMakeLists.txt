
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/elf_loader.cpp)

target_sources(simulator PRIVATE ${SOURCES})

target_include_directories(simulator PUBLIC ${PROJECT_BINARY_DIR}
                                         ${PROJECT_SOURCE_DIR}/third-party/ELFIO)
