#include <stdio.h>
#include <stdlib.h>

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
long long numberOfSplitInversons(int *arr, int start, int mid, int end) {
    int n1 = mid - start;
    int n2 = end - mid;
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++)
        L[i] = arr[start + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + j];

    int i = 0, j = 0, k = start, inv_count = 0;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
            inv_count += (n1 - i);
        }
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    free(L);
    free(R);
    return inv_count;
}

long long numbersofInversons(int *arr, int start, int end){
    if(end - start < 2) return 0;
    int mid = (start + end) / 2;
    long long left = numbersofInversons(arr, start, mid);
    long long right = numbersofInversons(arr, mid, end);
    long long split = numberOfSplitInversons(arr, start, mid, end);
    return left + right + split;
}

int main() {
    FILE *file;
    int *numbers;
    int count = 0;
    int capacity = 10;
    int temp;
    
    file = fopen("Integer.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    numbers = malloc(capacity * sizeof(int));
    if (numbers == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return 1;
    }

    while (fscanf(file, "%d", &temp) == 1) {
        if (count >= capacity) {
            capacity *= 2;
            numbers = realloc(numbers, capacity * sizeof(int));
            if (numbers == NULL) {
                printf("Memory reallocation failed!\n");
                fclose(file);
                return 1;
            }
        }
        
        numbers[count] = temp;
        count++;
    }
    
    fclose(file);
    
    printf("Successfully parsed %d numbers into array.\n", count);
    printf("First 10 numbers: ");
    for (int i = 0; i < (count < 10 ? count : 10); i++) {
        printf("%d ", numbers[i]);
    }
    if (count > 10) printf("...");
    printf("\n");
    long long sum = 0;
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    long long res = numbersofInversons(numbers, 0,count);
    printf("Number of inversions: %lld\n", res);
    free(numbers);
    
    return 0;
}