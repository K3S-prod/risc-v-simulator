#include <link.h>

int main();

int _start()
{
    void *main_ptr = main;
    __asm__ (
        "jalr ra, %0;"
        "ebreak"
        :
        : "r" (main_ptr)
    );
}

int fibbonaci(int num) {
    if (num == 1) {
        return 1;
    } else if (num == 2) {
        return 1;
    } else {
        return fibbonaci(num - 1) + fibbonaci(num - 2);
    }
}

int main() {
    int result = 0;
    result = fibbonaci(11);
    return result;
}
