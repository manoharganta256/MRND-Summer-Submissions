#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int sort_012(int *arr, int len) {
    int low = 0;
    int high = len - 1;
    int mid = 0;

    while (mid <= high) {
        switch (arr[mid]) {
            case 0:
                swap(&arr[low++], &arr[mid++]); break;
            case 1:
                mid++; break;
            case 2:
                swap(&arr[mid], &arr[high--]); break;
        }
    }
}

void print_array(int *arr, int len) {
    for(int i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[] = {0, 1, 2, 0, 1, 2, 0, 1, 2};
    int len = sizeof(arr) / sizeof(arr[0]);
    
    sort_012(arr, len);
    print_array(arr, len);
    return 0;
}