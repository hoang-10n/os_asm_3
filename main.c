#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mergesort.h"

int *A, *B, cutoff_level;

int main() {
    // Sample unsorted array
    int sample[] = {38, 27, 43, 3, 9, 82, 10, 17, 1, 14, 5, 60, 45, 8, 2, 19};
    int n = sizeof(sample) / sizeof(sample[0]);
    cutoff_level = 2;
    
    A = malloc(n * sizeof(int));
    B = malloc(n * sizeof(int));

    memcpy(A, sample, n * sizeof(int));

    struct argument *arg = buildArgs(0, n - 1, 0);
    parallel_mergesort(arg);

    printf("Sorted: ");
    for (int i = 0; i < n; i++)
        printf("%d ", A[i]);
    printf("\n");

    free(A);
    free(B);
    return 0;
}
