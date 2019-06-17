#include <stdio.h>
#include <stdlib.h>

char convert(int num) {
    if (num == 0)
        return '0';
    if (num == 1)
        return '1';
    if (num == 2)
        return '2';
    if (num == 3)
        return 'E';
    if (num == 4)
        return 'h';
    if (num == 5)
        return '5';
    if (num == 6)
        return '9';
    if (num == 7)
        return 'L';
    if (num == 8)
        return '8';
    if (num == 9)
        return '6';
}


void state_space(int *arr, int n, int index) {
    if (index == n - 1) {
        int leading_zero = 1;
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] != 0)
                leading_zero = 0;

            if (!leading_zero)
                printf("%c", convert(arr[i]));
        }
        printf("\n");
    }
    else {
        for (int number = 0; number <= 9; number++) {
            arr[index] = number;
            state_space(arr, n, index + 1);
        }
    }

}

int main() {
    int n;
    scanf("%d", &n);

    int *arr = (int *)malloc(sizeof(int) * n);
    state_space(arr, n, 0);
    return 0;
}