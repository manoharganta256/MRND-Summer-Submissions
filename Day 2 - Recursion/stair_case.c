#include <stdio.h>
#include <stdlib.h>

void stair_case_util(int *arr, int n, int k, int index) {

    if (index <= n) {
        arr[index] = 0;

        for (int i = 1; i <= n && i <= k; i++)
            arr[index] += arr[index - i];

        stair_case_util(arr, n, k, index + 1);
    }

}

int stair_case_recursion(int n, int k) {
    int *arr = (int *)malloc(sizeof(int) * (n + 1));
    int index = 2;

    arr[0] = 1;
    arr[1] = 1;
    stair_case_util(arr, n, k, index);
    return arr[n];
}

int stair_case_dp(int n, int k) {
    int *arr = (int *)malloc(sizeof(int) * (n + 1));
    arr[0] = 1;
    arr[1] = 1;
    for (int index = 2; index <= n ; index++) {
        arr[index] = 0;

        for (int i = 1; i <= n && i <= k; i++)
            arr[index] += arr[index - i];

    }

    return arr[n];
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);

    int result = stair_case_recursion(n, k);
    printf("%d", result);
    return 0;
}