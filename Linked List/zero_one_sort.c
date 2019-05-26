#include <stdio.h>
#include <stdlib.h>

void sort_count_ones(int *arr, int len) {
    int ones = 0;
    for (int i = 0; i < len; i++) {
        if (arr[i] == 1)
            ones++;
    }

    for (int i = 0; i < len - ones; i++)
        arr[i] = 0;

    for (int i = ones; i < len; i++)
        arr[i] = 1;
}

void sort(int *arr, int len) {
    int count[2];

    count[0] = 0;
    count[1] = 0;
    for (int i = 0; i < len; i++) {
        if (arr[i] == 0)
            count[0] += 1;
        else
            count[1] += 1;
    }

    int index = 0;
    while (count[0]--)
        arr[index++] = 0;
    while (count[1]--)
        arr[index++] = 1;
}

void swapping_sort(int *arr, int len) {
    int left = 0, right = len - 1;

    while (left <= right) {
        while (arr[left] == 0)
            left++;
        while (arr[right] == 1)
            right--;

        if (left < right) {
            arr[left] = 0;
            arr[right] = 1;
        }
    }
}

void print_array(int *arr, int len) {
    for(int i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int len;
    int arr[] = {0, 1, 0, 1, 0, 1};
    len = sizeof(arr) / sizeof(arr[0]);

    sort_count_ones(arr, len);
    print_array(arr, len);

    int arr1[] = {1, 0, 1, 0, 1, 0};
    len = sizeof(arr1) / sizeof(arr1[0]);

    sort(arr1, len);
    print_array(arr1, len);

    int arr2[] = {1, 0, 1, 0, 1, 0};
    len = sizeof(arr2) / sizeof(arr2[0]);

    swapping_sort(arr, len);
    print_array(arr, len);

    return 0;
}