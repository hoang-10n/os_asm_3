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
extern int cutoff;

/* Merge two sorted halves */
void merge(int leftStart, int leftEnd, int rightStart, int rightEnd) {
    int i = leftStart;
    int j = rightStart;
    int k = leftStart;

    while (i <= leftEnd && j <= rightEnd) {
        if (A[i] <= A[j]) {
            B[k++] = A[i++];
        } else {
            B[k++] = A[j++];
        }
    }

    while (i <= leftEnd) {
        B[k++] = A[i++];
    }

    while (j <= rightEnd) {
        B[k++] = A[j++];
    }

    // Copy merged data back to A
    memcpy(A + leftStart, B + leftStart, (rightEnd - leftStart + 1) * sizeof(int));
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

    if (level >= cutoff) {
        // If number of threads exceed cutoff value, sort sequentially
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

    // free(a);
    return NULL;
}
