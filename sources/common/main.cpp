
#include <string>
#include <iostream>

#include "common.h"


int main(int argc, char** argv) {

    const std::string Usage = "Usage: ./simulator <elf_filename>";
    if (argv[1] == NULL) {
        std::cout << Usage << std::endl;
        return EXIT_FAILURE;
    }

    std::string elfFileName = std::string(argv[1]);
    sim::State state;
    state.loadELF(elfFileName);

    state.getMemory().dump();

    return 0;
}
