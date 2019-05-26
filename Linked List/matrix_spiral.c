#include <stdio.h>
#include <stdlib.h>

int **get_matrix(int n, int m) {
    int **arr = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++)
        arr[i] = (int *)malloc(sizeof(int) * m);
    return arr;
}

int *spiral(int **arr, int row_end, int col_end) {
    int *result = (int *)malloc(sizeof(int) * (row_end * col_end));
    int index = 0, row_start = 0, col_start = 0;
    row_end--;
    col_end--;

    while (row_start <= row_end && col_start <= col_end) {
        for (int k = col_start; k <= col_end; k++)
            result[index++] = arr[row_start][k];

        row_start++;
        for (int k = row_start; k <= row_end; k++)
            result[index++] = arr[k][col_end];

        col_end--;
        for(int k = col_end; k >= col_start; k--)
            result[index++] = arr[row_end][k];

        row_end--;
        for (int k = row_end; k >= row_start; k--)
            result[index++] = arr[k][col_start];

        col_start++;
    }
    return result;
}

void spiral_recursive_util(int **arr, int row_start, int col_start, int row_end, int col_end, int *result, int *index) {
    for (int i = row_start; i <= col_end; i++) {
        result[*index] = arr[row_start][i];
        *index += 1;
    }

    for (int i = row_start + 1; i <= row_end; i++) {
        result[*index] = arr[i][col_end];
        *index += 1;
    }

    if (row_start + 1 <= row_end) {
        for (int i = col_end - 1; i >= col_start; i--) {
            result[*index] = arr[row_end][i];
            *index += 1;
        }
    }

    if (col_start + 1 <= col_end) {
        for (int  i = row_end - 1; i > row_start; i--) {
            result[*index] = arr[i][col_start];
            *index += 1;
        }
    }

    if (row_start + 1 < row_end && col_start + 1 < col_end)
        spiral_recursive_util(arr, row_start + 1, col_start + 1, row_end - 1, col_end - 1, result, index);
}

int *spiral_recursive(int **arr, int n, int m) {
    int *result = (int *)malloc(sizeof(int) * (n * m));
    int index = 0;

    spiral_recursive_util(arr, 0, 0, n - 1, m - 1, result, &index);
    return result;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    int **arr = get_matrix(n, m);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &arr[i][j]);

    int *spiral_arr;

    printf("----spiral----\n");
    spiral_arr = spiral(arr, n, m);
    for (int i = 0; i < n * m; i++)
        printf("%d ", spiral_arr[i]);

    printf("\n----spiral recursion----\n");
    spiral_arr =  spiral_recursive(arr, n, m);
    for (int i = 0; i < n * m; i++)
        printf("%d ", spiral_arr[i]);

    return 0;
}