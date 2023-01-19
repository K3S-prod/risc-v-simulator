

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
}