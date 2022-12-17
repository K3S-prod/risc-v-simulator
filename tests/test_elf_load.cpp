
#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include <cassert>
#include <sys/stat.h>

#include "common.h"


TEST(sim, ELF_load) {
    std::string elfFileName = std::string("fibbonaci");
    sim::State state(elfFileName);

    const char* refFilename = "fibbonaci_elf_segments_reference.sim";
    FILE* referenceFile = fopen(refFilename, "rb");
    assert(referenceFile != NULL);

    char* memoryRef = (char*) calloc(DRAM_SIZE, sizeof(char));
    struct stat st;
    stat(refFilename, &st);
    auto size = st.st_size;
    fread(memoryRef, sizeof(char), size, referenceFile);

    const std::string testFilename = "fibbonaci";
    sim::Memory memory;
    memory.loadELF(testFilename);

    free(memoryRef);
}
