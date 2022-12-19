
#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <sys/stat.h>

#include "common.h"


TEST(sim, ELF_load) {
    std::string elfFileName = std::string("../tests/fibbonacci");
    sim::Simulator state(elfFileName);

    const char* refFilename = "../tests/fibbonacci_decoded_ref.sim";
    FILE* referenceFile = fopen(refFilename, "rb");
    assert(referenceFile != NULL);

    char* memoryRef = (char*) calloc(DRAM_SIZE, sizeof(char));
    struct stat st;
    stat(refFilename, &st);
    auto size = st.st_size;
    fread(memoryRef, sizeof(char), size, referenceFile);

    const std::string testFilename = "../tests/fibbonacci";
    sim::Memory memory;
    memory.loadELF(testFilename);

    fclose(referenceFile);
    free(memoryRef);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
