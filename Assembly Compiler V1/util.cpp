
#include "util.h"

int isalpha(char c) {
    if (c >= 'A' && c <= 'Z')
        return 1;
    return 0;
}


int strlen_(char *str) {
    int len = 0;
    while (str[len] != '\n' && str[len])
        len++;
    return len;
}

void str_lower(char *str) {
    int index = 0;

    while (str[index]) {
        if (str[index] >= 'A' && str[index] <= 'B')
            str[index] = (str[index] - 'A') + 'a';
        index++;
    }
}

int str_compare(char *str1, char *str2) {
    int index = 0;
    while (str1[index] && str2[index]) {
        if (str1[index] != str2[index])
            return 0;
        index++;
    }

    if (!str1[index] && !str2[index])
        return 1;
    return 0;
}

void fillArray(int *arr, int len, int val) {
    for (int i = 0; i < len; i++)
        arr[i] = val;
}
