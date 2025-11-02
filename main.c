#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "mergesort.h"

int *A, *B, cutoff;

int main() {
    // Unsorted array
    int sample[] = {38, 27, 43, 3, 9, 82, 10, 17, 1, 14, 5, 60, 45, 8, 2, 19, 38, 27, 43, 3, 9, 82, 10, 17, 1, 14, 5, 60, 45, 8, 2, 19};
    int n = sizeof(sample) / sizeof(sample[0]);
    cutoff = 10;

    // Allocate arrays
    A = malloc(n * sizeof(int));
    B = malloc(n * sizeof(int));
    if (!A || !B) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Run sequential mergesort
    memcpy(A, sample, n * sizeof(int));
    clock_t start_seq = clock();
    my_mergesort(0, n - 1);
    clock_t end_seq = clock();
    double time_seq = (double)(end_seq - start_seq) / CLOCKS_PER_SEC;

    int *sorted_seq = malloc(n * sizeof(int));
    memcpy(sorted_seq, A, n * sizeof(int));

    // Run parallel mergesort
    memcpy(A, sample, n * sizeof(int));
    struct argument *arg = buildArgs(0, n - 1, 0);

    clock_t start_par = clock();
    parallel_mergesort(arg);
    clock_t end_par = clock();
    double time_par = (double)(end_par - start_par) / CLOCKS_PER_SEC;

    // Check if the results are identical
    int match = 1;
    for (int i = 0; i < n; i++) {
        if (A[i] != sorted_seq[i]) {
            match = 0;
            break;
        }
    }

    // Print the results
    printf("Sequential sort time: %.6f seconds\n", time_seq);
    printf("Parallel sort time:   %.6f seconds\n", time_par);
    printf("Results match: %s\n\n", match ? "YES" : "NO");

    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", A[i]);
    printf("\n");

    // Clean up and free the arrays
    free(A);
    free(B);
    free(sorted_seq);

    return 0;
}
