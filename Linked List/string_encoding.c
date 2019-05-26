#include <stdio.h>

int digits(int number) {
    int count = 0;
    while (number > 0) {
        count++;
        number /= 10;
    }
    return count;
}

void encode_inplace(char *str) {
    char current = *str;
    int index = 0, position = 0, count = 0, r_index, digit_count;
    while (1) {
        if (current != str[index]) {
            str[position] = current;
            if (count == 1) {
                position += 1;
                if (str[index] == '\0')
                    break;
                current = str[index];
                count = 0;
                continue;
            }

            digit_count = digits(count);
            r_index = position + digit_count;

            while (r_index != position) {
                str[r_index--] = (char)((count % 10) + '0');
                count /= 10;
            }
            position += digit_count + 1;

            if (str[index] == '\0')
                break;
            current = str[index];
            count = 0;
        }
        else {
            index++;
            count++;
        }
    }
    str[position] = '\0';
}

int main(){
    char str[1000];
    scanf("%s", str);
    encode_inplace(str);
    printf("%s", str);
    return 0;
}
