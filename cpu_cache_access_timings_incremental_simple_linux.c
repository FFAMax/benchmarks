#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

int main() {
    // Initialize variables
    int size = 16 * 1024; // Initial size: 16 KB
    int max_size = 1024 * 1024 * 1024; // Maximum size: 1 GB
    int iterations = 10000; // Number of iterations for each size
    int increment = 1024; // Increment size

    while (size <= max_size) {
        // Allocate memory
        char* data = malloc(size);
        if (!data) {
            printf("Memory allocation failed\n");
            return -1;
        }

        // Initialize memory with random values
        for (int i = 0; i < size; i++) {
            data[i] = rand() % 256;
        }

        // Measure access time
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < iterations; i++) {
            int index = rand() % size;
            char value = data[index]; // Read a byte from a random address
            // Prevent compiler optimization
            if (value == 0) {
                // Do nothing
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate elapsed time in nanoseconds
        uint64_t elapsed_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        double average_access_time = (double)elapsed_ns / iterations;

        // Print results
        printf("Size: %d KB, Average Access Time: %.2f ns\n", size / 1024, average_access_time);

        // Free allocated memory
        free(data);

        // Increment size by 1 MB
        size += increment;
    }

    return 0;
}
