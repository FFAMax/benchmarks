#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fp;
    fp = fopen("output-c.txt", "w");
    srand(time(0)); // Инициализация генератора случайных чисел
    for (int i = 0; i < 1000000; i++) {
        int number = 900000000 + rand() % 100000000;
        fprintf(fp, "%d\n", number);
    }
    fclose(fp);
    return 0;
}

// apt-get install build-essential
// gcc source_file.c -o executable_file
// gcc hello.c -o hello
// ./hello

