#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <mach/mach_time.h>

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
        uint64_t start_time = mach_absolute_time();
        for (int i = 0; i < iterations; i++) {
            int index = rand() % size;
            char value = data[index]; // Read a byte from a random address
            // Do something with the value to prevent compiler optimization
            if (value == 0) {
                // Do nothing
            }
        }
        uint64_t end_time = mach_absolute_time();

        // Calculate average access time in nanoseconds
        static mach_timebase_info_data_t s_timebase_info;
        if (s_timebase_info.denom == 0) {
            mach_timebase_info(&s_timebase_info);
        }
        double average_access_time = ((double)(end_time - start_time) * s_timebase_info.numer / s_timebase_info.denom) / iterations;

        // Print results
        printf("Size: %d KB, Average Access Time: %.2f ns\n", size / 1024, average_access_time);

        // Free allocated memory
        free(data);

        // Increment size by 1 MB
        size += increment;
    }

    return 0;
}

// Size: 32075 KB, Average Access Time: 32.23 ns
// Size: 32095 KB, Average Access Time: 49.05 ns
// Size: 32115 KB, Average Access Time: 30.88 ns
// Size: 32135 KB, Average Access Time: 32.68 ns
