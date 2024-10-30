#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>

uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

void update_top_cycles(uint64_t cycles, uint64_t *top_cycles, int *top_counts) {
    for (int i = 0; i < 10; i++) {
        if (cycles == top_cycles[i]) {
            top_counts[i]++;
            return;
        } else if (cycles < top_cycles[i] || top_cycles[i] == 0) {
            // Shift elements to the right
            for (int j = 9; j > i; j--) {
                top_cycles[j] = top_cycles[j - 1];
                top_counts[j] = top_counts[j - 1];
            }
            top_cycles[i] = cycles;
            top_counts[i] = 1;
            return;
        }
    }
}

void test_access_time(size_t size_in) {
    size_t size = size_in * 1024 * 100; // Steps by 100K
    int *array = (int*)malloc(size * sizeof(int));
    if (!array) {
        perror("Failed to allocate memory");
        return;
    }

    // Initialize the array
    for (size_t i = 0; i < size; i++) {
        array[i] = rand();
    }

    const int iterations = 10000000;
    int index;
    uint64_t start, end;
    uint64_t total_cycles = 0, min_cycles = UINT64_MAX, max_cycles = 0;

    uint64_t low_cycles[10] = {0};
    int low_counts[10] = {0};
    uint64_t high_cycles[10] = {0};
    int high_counts[10] = {0};

    // Measure access times
    for (int i = 0; i < iterations; i++) {
        index = rand() % size;

        start = rdtsc();
        array[index]++;
        end = rdtsc();

        uint64_t cycles = end - start;
        total_cycles += cycles;

        if (cycles < min_cycles) min_cycles = cycles;
        if (cycles > max_cycles) max_cycles = cycles;

        // Update top low and high cycle statistics
        update_top_cycles(cycles, low_cycles, low_counts);
        update_top_cycles(-cycles, high_cycles, high_counts);
    }

    free(array);

    double average_cycles = (double)total_cycles / iterations;

    // Print summary of results
    printf("Size: %zu K, Min: %llu cycles, Max: %llu cycles, Avg: %.2f cycles\n",
           size_in * 100, min_cycles, max_cycles, average_cycles);

    // Print top 10 low cycles with percentages
    printf("Top 10 Low Cycles:\n");
    for (int i = 0; i < 10 && low_cycles[i] != 0; i++) {
        double percentage = (double)low_counts[i] / iterations * 100;
        printf("  %llu cycles: %d times (%.2f%%)\n", low_cycles[i], low_counts[i], percentage);
    }

    // Print top 10 high cycles with percentages
    printf("Top 10 High Cycles:\n");
    for (int i = 0; i < 10 && high_cycles[i] != 0; i++) {
        double percentage = (double)high_counts[i] / iterations * 100;
        printf("  %llu cycles: %d times (%.2f%%)\n", -high_cycles[i], high_counts[i], percentage);
    }
}

int main() {
    for (size_t in = 1; in <= 10000; in++) { // Adjust limit as needed
        test_access_time(in);
    }
    return 0;
}

// clang -O0  -Wall -o cache_benchmark 5.c &&  ./cache_benchmark
/*
bash-3.2$ clang -O0  -Wall -o cache_benchmark 5.c &&  ./cache_benchmark
Size: 100 K, Min: 12 cycles, Max: 97296 cycles, Avg: 21.19 cycles
Top 10 Low Cycles:
  12 cycles: 30434 times (0.30%)
  14 cycles: 2542609 times (25.43%)
  16 cycles: 3190673 times (31.91%)
  18 cycles: 336489 times (3.36%)
  19 cycles: 840 times (0.01%)
  20 cycles: 642371 times (6.42%)
  21 cycles: 8283 times (0.08%)
  22 cycles: 971653 times (9.72%)
  24 cycles: 114097 times (1.14%)
  25 cycles: 105916 times (1.06%)
Top 10 High Cycles:
  97296 cycles: 1 times (0.00%)
  78886 cycles: 1 times (0.00%)
  65314 cycles: 1 times (0.00%)
  63808 cycles: 1 times (0.00%)
  59816 cycles: 1 times (0.00%)
  59394 cycles: 1 times (0.00%)
  58438 cycles: 1 times (0.00%)
  52644 cycles: 1 times (0.00%)
  52472 cycles: 1 times (0.00%)
  51424 cycles: 1 times (0.00%)
Size: 200 K, Min: 12 cycles, Max: 218959 cycles, Avg: 21.95 cycles
Top 10 Low Cycles:
  12 cycles: 23325 times (0.23%)
  14 cycles: 2395628 times (23.96%)
  16 cycles: 3237812 times (32.38%)
  18 cycles: 369159 times (3.69%)
  19 cycles: 650 times (0.01%)
  20 cycles: 396473 times (3.96%)
  21 cycles: 7222 times (0.07%)
  22 cycles: 878567 times (8.79%)
  24 cycles: 134604 times (1.35%)
  25 cycles: 264549 times (2.65%)
Top 10 High Cycles:
  218959 cycles: 1 times (0.00%)
  80224 cycles: 1 times (0.00%)
  78294 cycles: 1 times (0.00%)
  75922 cycles: 1 times (0.00%)
  66817 cycles: 1 times (0.00%)
  43276 cycles: 1 times (0.00%)
  39776 cycles: 1 times (0.00%)
  38476 cycles: 1 times (0.00%)
  37572 cycles: 1 times (0.00%)
  37306 cycles: 1 times (0.00%)
  */
