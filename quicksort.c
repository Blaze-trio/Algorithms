#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

// Lecture partition: pivot at array[low], i=low+1, j=low+1..high, final swap
int partition(int *array, int low, int high) {
    int pivot = array[low];
    int i = low + 1;
    for (int j = low + 1; j <= high; j++) {
        if (array[j] < pivot) {
            swap(&array[i], &array[j]);
            i++;
        }
    }
    swap(&array[low], &array[i - 1]);
    return i - 1;
}

void quicksort_first(int *array, int low, int high, long long *comparisons) {
    if (low >= high) return;
    *comparisons += (high - low);                 // m-1
    int p = partition(array, low, high);
    quicksort_first(array, low, p - 1, comparisons);
    quicksort_first(array, p + 1, high, comparisons);
}

// Last-element pivot: swap last into first, then use the same partition
void quicksort_last(int *array, int low, int high, long long *comparisons) {
    if (low >= high) return;
    *comparisons += (high - low);
    swap(&array[low], &array[high]);              // move last to first
    int p = partition(array, low, high);
    quicksort_last(array, low, p - 1, comparisons);
    quicksort_last(array, p + 1, high, comparisons);
}

// Median-of-three pivot index (lower middle for even length)
static inline int median_of_three_index(int *A, int l, int r) {
    int m = l + (r - l) / 2;
    int a = A[l], b = A[m], c = A[r];
    if ((a <= b && b <= c) || (c <= b && b <= a)) return m;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return l;
    return r;
}

void quicksort_median(int *array, int low, int high, long long *comparisons) {
    if (low >= high) return;
    *comparisons += (high - low);
    int pivot_idx = median_of_three_index(array, low, high);
    swap(&array[low], &array[pivot_idx]);         // move chosen pivot to first
    int p = partition(array, low, high);
    quicksort_median(array, low, p - 1, comparisons);
    quicksort_median(array, p + 1, high, comparisons);
}

int main(void) {
    const char *path = "array.txt";
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", path);
        return 1;
    }

    int capacity = 1024, count = 0, val;
    int *numbers = (int *)malloc(capacity * sizeof(int));
    if (!numbers) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    while (fscanf(file, "%d", &val) == 1) {
        if (count >= capacity) {
            capacity *= 2;
            int *tmp = (int *)realloc(numbers, capacity * sizeof(int));
            if (!tmp) {
                fprintf(stderr, "Memory reallocation failed\n");
                free(numbers);
                fclose(file);
                return 1;
            }
            numbers = tmp;
        }
        numbers[count++] = val;
    }
    fclose(file);

    // Work on copies so each rule sees the same input
    int *a1 = malloc(count * sizeof(int));
    int *a2 = malloc(count * sizeof(int));
    int *a3 = malloc(count * sizeof(int));
    if (!a1 || !a2 || !a3) {
        fprintf(stderr, "Memory allocation failed\n");
        free(numbers); free(a1); free(a2); free(a3);
        return 1;
    }
    memcpy(a1, numbers, count * sizeof(int));
    memcpy(a2, numbers, count * sizeof(int));
    memcpy(a3, numbers, count * sizeof(int));

    long long c1 = 0, c2 = 0, c3 = 0;

    quicksort_first(a1, 0, count - 1, &c1);   // first element pivot
    quicksort_last(a2, 0, count - 1, &c2);    // last element pivot
    quicksort_median(a3, 0, count - 1, &c3);  // median-of-three

    // Print counts (one per line). For the grader, submit the line needed.
    printf("%lld\n", c1);
    printf("%lld\n", c2);
    printf("%lld\n", c3);

    free(numbers); free(a1); free(a2); free(a3);
    return 0;
}