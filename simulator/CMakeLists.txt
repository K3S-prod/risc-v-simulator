add_subdirectory(decoder)
add_subdirectory(interpreter)
add_subdirectory(memory)
add_subdirectory(elf_loader)

set(SOURCES simulator.cpp)
add_library(simulator simulator.cpp)
target_link_libraries(simulator PUBLIC interpreter decoder memory elf_loader)

add_executable(rvsim main.cpp)
target_link_libraries(rvsim PUBLIC simulator)
