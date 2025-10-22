/**
 * Parallel Merge Sort Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mergesort.h"

/* External arrays from main */
extern int *A;
extern int *B;
extern int cutoff_level;

/* Merge two sorted halves */
void merge(int leftstart, int leftend, int rightstart, int rightend) {
    int i = leftstart;
    int j = rightstart;
    int k = leftstart;

    while (i <= leftend && j <= rightend) {
        if (A[i] <= A[j])
            B[k++] = A[i++];
        else
            B[k++] = A[j++];
    }

    while (i <= leftend)
        B[k++] = A[i++];

    while (j <= rightend)
        B[k++] = A[j++];

    // Copy merged data back to A
    memcpy(A + leftstart, B + leftstart, (rightend - leftstart + 1) * sizeof(int));
}

/* Sequential merge sort */
void my_mergesort(int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;

    my_mergesort(left, mid);
    my_mergesort(mid + 1, right);
    merge(left, mid, mid + 1, right);
}

/* Build thread argument */
struct argument * buildArgs(int left, int right, int level) {
    struct argument *arg = malloc(sizeof(struct argument));
    arg->left = left;
    arg->right = right;
    arg->level = level;
    return arg;
}

/* Parallel merge sort */
void * parallel_mergesort(void *arg) {
    struct argument *a = (struct argument *)arg;
    int left = a->left;
    int right = a->right;
    int level = a->level;

    if (left >= right) {
        free(a);
        return NULL;
    }

    int mid = (left + right) / 2;

    if (level >= cutoff_level) {
        // Base case: sort sequentially
        my_mergesort(left, right);
    } else {
        pthread_t t1, t2;

        // Build arguments for two halves
        struct argument *leftArgs = buildArgs(left, mid, level + 1);
        struct argument *rightArgs = buildArgs(mid + 1, right, level + 1);

        // Create two threads
        pthread_create(&t1, NULL, parallel_mergesort, leftArgs);
        pthread_create(&t2, NULL, parallel_mergesort, rightArgs);

        // Wait for both to finish
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        // Merge results
        merge(left, mid, mid + 1, right);
    }

    free(a);
    return NULL;
}
