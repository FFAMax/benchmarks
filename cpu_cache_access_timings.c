#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>
#include <string.h>

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

void write_csv_header(FILE *file, const char *label, uint64_t *cycles) {
    fprintf(file, "%s", label);
    for (int i = 0; i < 10 && cycles[i] != 0; i++) {
        fprintf(file, ",%llu", cycles[i]);
    }
    fprintf(file, "\n");
}

void write_csv_data(FILE *file, const char *label, int *counts, int total_iterations, int is_percentage) {
    fprintf(file, "%s", label);
    for (int i = 0; i < 10 && counts[i] != 0; i++) {
        double value = is_percentage ? (counts[i] * 100.0 / total_iterations) : counts[i];
        fprintf(file, ",%.2f", value);
    }
    fprintf(file, "\n");
}

void test_access_time(size_t size_in, int csv_mode, FILE *abs_file, FILE *percent_file) {
    size_t size = size_in * 1024 * 10; // Steps by 10K
    int *array = (int*)malloc(size * sizeof(int));
    if (!array) {
        perror("Failed to allocate memory");
        return;
    }

    // Initialize the array
    for (size_t i = 0; i < size; i++) {
        array[i] = rand();
    }

    const int iterations = 100000000;
    int index;
    uint64_t start, end;
    uint64_t total_cycles = 0;

    uint64_t low_cycles[10] = {0};
    int low_counts[10] = {0};

    // Measure access times
    for (int i = 0; i < iterations; i++) {
        index = rand() % size;

        start = rdtsc();
        array[index]++;
        end = rdtsc();

        uint64_t cycles = end - start;
        total_cycles += cycles;

        // Update top low cycle statistics
        update_top_cycles(cycles, low_cycles, low_counts);
    }

    free(array);

    // CSV output mode
    if (csv_mode) {
        char size_label[32];
        snprintf(size_label, sizeof(size_label), "Size_%zuK", size_in * 10);

        // Write headers for first iteration
        if (ftell(abs_file) == 0) {
            write_csv_header(abs_file, "Size(KB)", low_cycles);
            write_csv_header(percent_file, "Size(KB)", low_cycles);
        }

        // Write data for each iteration
        write_csv_data(abs_file, size_label, low_counts, iterations, 0);
        write_csv_data(percent_file, size_label, low_counts, iterations, 1);
    } else {
        // Print summary for non-CSV mode
        printf("Size: %zu K\n", size_in * 100);
        for (int i = 0; i < 10 && low_cycles[i] != 0; i++) {
            printf("  %llu cycles: %d times\n", low_cycles[i], low_counts[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    int csv_mode = (argc > 1 && strcmp(argv[1], "--csv") == 0);
    FILE *abs_file = NULL;
    FILE *percent_file = NULL;

    if (csv_mode) {
        abs_file = fopen("low_cycles_abs.csv", "w");
        percent_file = fopen("low_cycles_percent.csv", "w");
        if (!abs_file || !percent_file) {
            perror("Failed to open CSV files");
            return 1;
        }
    }

    for (size_t in = 1; in <= 100; in++) { // Adjust limit as needed
        test_access_time(in, csv_mode, abs_file, percent_file);
    }

    if (csv_mode) {
        fclose(abs_file);
        fclose(percent_file);
    }

    return 0;
}

// usage 
// clang -O0  -Wall -o cache_benchmark 9.c &&  ./cache_benchmark  --csv
//
// Example of output 
// bash-3.2$ cat low_cycles_percent.csv
// Size(KB),12,14,16,18,19,20,21,22,24,25
// Size_10K,0.19,5.53,24.18,18.07,0.02,4.35,0.03,9.60,5.27,2.86
// Size_20K,0.13,5.81,25.52,18.06,0.01,4.51,0.04,8.42,3.54,4.10
// Size_30K,0.07,6.38,28.77,20.28,0.00,4.91,0.02,6.64,2.65,2.46
// Size_40K,0.08,6.60,29.24,20.52,0.00,4.98,0.02,6.41,2.58,2.07
//
// bash-3.2$ head low_cycles_abs.csv
// Size(KB),12,14,16,18,19,20,21,22,24,25
// Size_10K,193892.00,5527168.00,24180059.00,18072871.00,18759.00,4351860.00,34418.00,9599463.00,5265940.00,2856736.00
// Size_20K,129191.00,5814628.00,25516310.00,18063141.00,10693.00,4513953.00,37764.00,8421835.00,3541524.00,4100560.00
// Size_30K,68985.00,6378547.00,28768910.00,20277251.00,4805.00,4910176.00,19090.00,6637525.00,2646383.00,2461273.00
// Size_40K,76527.00,6600617.00,29239346.00,20519510.00,4568.00,4977020.00,20545.00,6410067.00,2582119.00,2066451.00
// 
// Google colab grath - https://colab.research.google.com/drive/1HkwFOEaR2OuYGJCeFLMTF8FmjluFeTQv?usp=sharing
