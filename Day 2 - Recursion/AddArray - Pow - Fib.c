#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100

long add(int *arr, int len) {
    return len < 0 ? 0 : arr[len] + add(arr, len - 1);
}

int pow_(int a, int b) {
    if (b == 0)
        return 1;
    else if (b % 2 == 0)
        return (pow_(a, b / 2) * pow_(a, b / 2)) % 1000000007;
    else
        return (a * pow_(a, b / 2) * pow_(a, b / 2)) % 1000000007;
}

long fib(int n, int *arr) {
    if (arr[n] == -1)
       arr[n] = fib(n - 1, arr) + fib(n - 2, arr);
    return arr[n];
}

int main() {
//    int arr[] = {1, 2, 3};
//    int len = sizeof(arr) / sizeof(arr[0]);

//    long sum = add(arr, len - 1);

//    int n;
//    scanf("%d", &n);
//    int *arr = (int *)malloc(sizeof(int) * MAXSIZE);
//
//    for (int i = 0; i < MAXSIZE; i++)
//        arr[i] = -1;
//    arr[0] = 0; arr[1] = 1;
//
//    int result = fib(n, arr);
//    printf("%d", result);
//    free(arr);

    printf("%d", pow_(11, 9));
    return 0;
}