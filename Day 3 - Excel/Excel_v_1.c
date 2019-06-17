#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10

struct command {
    int type;
    char *cell;
    int val;
    char *file_name;
};

typedef struct command Command;

int is_digit(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

void str_upper(char *token) {
    for (int i = 0; token[i]; i++) {
        if (token[i] >= 'a' && token[i] <= 'z')
            token[i] = token[i] - 'a' + 'A';
    }
}

int **create_sheet() {
    int **sheet = (int **)malloc(sizeof(int *) * MAXSIZE);
    for (int i = 0; i < MAXSIZE; i++)
        sheet[i] = (int *)malloc(sizeof(int) * MAXSIZE);

    for (int i = 0; i < MAXSIZE; i++) {
        for (int j = 0; j < MAXSIZE; j++)
            sheet[i][j] = 0;
    }

    return sheet;
}

Command *create_command() {
    Command *command = (Command *)malloc(sizeof(command));

    command -> cell = (char *)malloc(sizeof(char) * 4);
    command -> file_name = (char *)malloc(sizeof(char) * 30);

    return command;
}

int str_compare(const char *str1,const char *str2) {
    int index = 0;

    for (; str1[index] && str2[index]; index++) {
        if (str1[index] != str2[index])
            return 0;
    }

    if (!str1[index] && !str2[index])
        return 1;
    return 0;
}

int type (char *token) {
    str_upper(token);

    if (str_compare(token, "GET"))
        return 1;

    if (str_compare(token, "SET"))
        return 2;

    if (str_compare(token, "PRINT"))
        return 3;

    if (str_compare(token, "IMPORT"))
        return 4;

    if (str_compare(token, "EXPORT"))
        return 5;

    return -1;
}

Command *parse_command(const char *input) {
    Command *command = create_command();

    char *token = (char *)malloc(sizeof(char) * 7);

    int index = 0;
    for (; input[index]; index++) {
        if (input[index] == ' ' || input[index] == '\t')
            break;

        token[index] = input[index];
    }
    token[index] = '\0';

    command -> type = type(token);

    if (command -> type == -1) {
        command -> type = -1;
        return command;
    }

    if (!input[index]) {
        command -> type = 3;
        return command;
    }

    while (input[index] == ' ' || input[index] == '\t')
        index++;

    if (command -> type == 1 || command -> type  == 2) {
        int cell_index = 0;
        command -> cell[cell_index++] = input[index++];
        command -> cell[cell_index++] = input[index++];

        if (is_digit(input[index]))
            command -> cell[cell_index++] = input[index++];

        command -> cell[cell_index] = '\0';

        if (command -> type  == 2) {
            if (input[index] == '=')
                index++;

            int num = 0;

            while (input[index]) {
                num *= 10;
                num += (int) (input[index] - '0');
                index++;
            }

            command -> val = num;
            command -> type = 2;
        }
    }

    else if (command -> type  == 4 || command -> type  == 5) {
        int file_index = 0;
        while(input[index]) {
            command -> file_name[file_index++] = input[index++];
        }
        command -> file_name[file_index] = '\0';
    }

    return command;
}

int GET(Command *command, int **sheet) {
    int col = command -> cell[0] - 'A';

    int row;
    if (is_digit(command -> cell[2]))
        row = 9;
    else
        row = (command -> cell[1] - '0') - 1;

    return sheet[row][col];
}

void SET(Command *command, int **sheet) {
    int col = command -> cell[0] - 'A';

    int row;
    if (is_digit(command -> cell[2]))
        row = 9;
    else
        row = (command -> cell[1] - '0') - 1;

    sheet[row][col] = command -> val;
}

void PRINT(int **sheet) {
    for (int i = 0; i < MAXSIZE; i++) {
        for (int j = 0; j < MAXSIZE; j++)
            printf("%d ", sheet[i][j]);

        printf("\n");
    }
}

void IMPORT(Command *command, int **sheet) {
    FILE *fp = fopen(command -> file_name, "r");

    for (int i = 0; i < MAXSIZE; i++) {
        for (int j = 0; j < MAXSIZE; j++) {
            int val;
            fscanf(fp, "%d", &val);

            sheet[i][j] = val;
            char c;
            fscanf(fp, "%c", &c);
        }
    }
    fclose(fp);
}

void EXPORT(Command *command, int **sheet) {
    FILE *fp = fopen(command -> file_name, "w");

    for (int i = 0; i < MAXSIZE; i++) {
        for (int j = 0; j < MAXSIZE; j++) {
            fprintf(fp, "%d", sheet[i][j]);
            if (j != MAXSIZE - 1)
                fprintf(fp, "%c", ',');
        }
        fprintf(fp, "%c", '\n');
    }

    fclose(fp);
}

int main() {
    int **sheet = create_sheet();

    while (1) {
        printf(">");
        char *input = (char *)malloc(sizeof(char) * 20);
        gets(input);

        Command *command = parse_command(input);

        if (command -> type == -1)
            break;

        if (command -> type == 1)
            printf("%d\n", GET(command, sheet));

        else if (command -> type == 2)
            SET(command, sheet);

        else if (command -> type == 3)
            PRINT(sheet);

        else if(command -> type == 4)
            IMPORT(command, sheet);
        else
            EXPORT(command, sheet);

    }

    return 0;
}