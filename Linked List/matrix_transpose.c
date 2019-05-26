#include <stdio.h>
#include <stdlib.h>

int **get_matrix(int n, int m) {
    int **arr = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++)
        arr[i] = (int *)malloc(sizeof(int) * m);
    return arr;
}

int **transpose(int **arr, int n, int m) {
    int **result = get_matrix(m, n);

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            result[i][j] = arr[j][i];

    return result;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    int **arr = get_matrix(n, m);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &arr[i][j]);

    int **arr_T = transpose(arr, n, m);

    printf("----Transpose----\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", arr_T[i][j]);
        printf("\n");
    }

    return 0;
}