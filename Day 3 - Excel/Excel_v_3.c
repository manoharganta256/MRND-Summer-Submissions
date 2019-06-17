#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10

struct command {
    int val;
    int type;
    char *cell;
    char *expression;
    char *file_name;
};

struct cell {
    int data;
    char *expression;
};

typedef struct cell Cell;
typedef struct command Command;

int GET(char *cell, Cell **sheet);

int is_digit(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

int is_number(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!is_digit(str[i]))
            return 0;
    }
    return 1;
}

int is_operator(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return 1;
    else
        return 0;
}

int str_to_number(const char *str) {
    int num = 0, index = 0;

    while (str[index]) {
        num *= 10;
        num += (int) (str[index] - '0');
        index++;
    }
    return num;
}

void str_upper(char *token) {
    for (int i = 0; token[i]; i++) {
        if (token[i] >= 'a' && token[i] <= 'z')
            token[i] = token[i] - 'a' + 'A';
    }
}

Cell **create_sheet() {
    Cell **sheet = (Cell **)malloc(sizeof(Cell *) * MAXSIZE);
    for (int i = 0; i < MAXSIZE; i++)
        sheet[i] = (Cell *)malloc(sizeof(Cell) * MAXSIZE);

    for (int i = 0; i < MAXSIZE; i++) {
        for (int j = 0; j < MAXSIZE; j++) {
            sheet[i][j].data = 0;
            sheet[i][j].expression = NULL;
        }
    }

    return sheet;
}

Command *create_command() {
    Command *command = (Command *)malloc(sizeof(command));

    command -> cell = (char *)malloc(sizeof(char) * 4);
    command -> file_name = (char *)malloc(sizeof(char) * 30);
    command -> expression = NULL;

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

int type(char *token) {
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

    if (str_compare(token, "SAVE"))
        return 6;

    if (str_compare(token, "EXIT"))
        return 0;


    return -1;
}

int eval(char *expression, Cell **sheet) {
    char left[10], right[10];
    int index = 0,left_val, right_val;
    while (!is_operator(*expression) && *expression) {
        left[index++] = *expression;
        expression++;
    }

    left[index] = '\0';
    if (is_number(left))
        left_val = str_to_number(left);
    else
        left_val = GET(left, sheet);

    if (!*expression)
        return left_val;

    char op = *expression++;

    index = 0;
    while (*expression) {
        right[index++] = *expression;
        expression++;
    }

    right[index] = '\0';

    if (is_number(right))
        right_val = str_to_number(right);
    else
        right_val = GET(right, sheet);

    if (op == '+')
        return left_val + right_val;
    if (op == '-')
        return left_val - right_val;
    if (op == '*')
        return left_val * right_val;
    if (op == '/')
        return left_val / right_val;

}

int detect_cycle(char *expression, Cell **sheet, int actual_row, int actual_col) {
    char visited[10][10], cell[10];
    int top = -1, index = 0;

    do {
        if (top != -1) {
            int col = visited[top][0] - 'A';

            int row;
            if (is_digit(visited[top][2]))
                row = 9;
            else
                row = (visited[top][1] - '0') - 1;

            if (row == actual_row && col == actual_col)
                return 1;

            top--;

            if (sheet[row][col].expression == NULL)
                continue;
            expression = sheet[row][col].expression;
        }

        while (!is_operator(*expression) && *expression) {
            cell[index++] = *expression;
            expression++;
        }

        cell[index] = '\0';
        if (!is_number(cell)) {
            top++;

            index = 0;
            for (; cell[index]; index++)
                visited[top][index] = cell[index];
            visited[top][index] = '\0';

        }

        if (*expression) {
            expression++;
            index = 0;
            while (!is_operator(*expression) && *expression) {
                cell[index++] = *expression;
                expression++;
            }

            cell[index] = '\0';
            if (!is_number(cell)) {
                top++;

                index = 0;
                for (; cell[index]; index++)
                    visited[top][index] = cell[index];
                visited[top][index] = '\0';

            }
        }

    } while (top != -1);

    return 0;

}

Command *parse_command(char *input) {
    Command *command = create_command();

    char *token = (char *)malloc(sizeof(char) * 10);

    int index = 0;
    while (*input) {
        if (*input == ' ' || *input == '\t')
            break;

        token[index++] = *input++;
    }
    token[index] = '\0';

    command -> type = type(token);

    if (command -> type == 0 || command -> type == -1 || command -> type == 6)
        return command;

    if (!*input) {
        command -> type = 3;
        return command;
    }

    while (*input == ' ' || *input == '\t')
        input++;

    if (command -> type == 1 || command -> type  == 2) {
        index = 0;
        command -> cell[index++] = *input++;
        command -> cell[index++] = *input++;

        if (is_digit(*input))
            command -> cell[index++] = *input++;

        command -> cell[index] = '\0';

        if (command -> type  == 2) {
            if (*input == '=')
                input++;

            if (is_number(input))
                command -> val =  str_to_number(input);
            else
                command -> expression =  input;
        }
    }

    else if (command -> type  == 4 || command -> type  == 5) {
        index = 0;
        int extension = 0;
        while(*input) {
            if (*input == '.')
                extension = 1;
            command -> file_name[index++] = *input++;
        }
        if (!extension) {
            command -> file_name[index++] = '.';
            command -> file_name[index++] = 'c';
            command -> file_name[index++] = 's';
            command -> file_name[index++] = 'v';
        }
        command -> file_name[index] = '\0';
    }

    return command;
}

int GET(char *cell, Cell **sheet) {
    int col = cell[0] - 'A';

    int row;
    if (is_digit(cell[2]))
        row = 9;
    else
        row = (cell[1] - '0') - 1;

    if (sheet[row][col].expression == NULL)
        return sheet[row][col].data;
    else
        return eval(sheet[row][col].expression, sheet);
}

void SET(Command *command, Cell **sheet) {
    int col = command -> cell[0] - 'A';

    int row;
    if (is_digit(command -> cell[2]))
        row = 9;
    else
        row = (command -> cell[1] - '0') - 1;

    if (command -> expression == NULL)
        sheet[row][col].data = command -> val;
    else {
        if (detect_cycle(command -> expression, sheet, row, col))
            printf("Given expression forms cycle\n");
        else
            sheet[row][col].expression = command -> expression;
    }
}

void PRINT(Cell **sheet) {
    for (int i = 0; i < MAXSIZE; i++) {
        for (int j = 0; j < MAXSIZE; j++) {
            if (sheet[i][j].expression == NULL)
                printf("%d ", sheet[i][j].data);
            else
                printf("%d ", eval(sheet[i][j].expression, sheet));
        }

        printf("\n");
    }
}

void IMPORT(Command *command, Cell **sheet) {
    FILE *fp = fopen(command -> file_name, "r");
    char line[256];
    int row = 0, col, num;
    while(fgets(line, 256, fp)) {
        col = 0;
        for (int i = 0; line[i]; i++) {
            num = 0;
            while (line[i] != ':' && line[i] != ',' && line[i] != '\n') {
                num *= 10;
                num += (int)(line[i] - '0');
                i++;
            }
            sheet[row][col].data = num;
            if (line[i] == '\n')
                break;
            if (line[i] == ',') {
                col++;
                continue;
            }
            i++;

            char *expression = (char *)malloc(sizeof(char) * 10);
            int index = 0;
            while (line[i] != ',' && line[i] != '\n') {
                expression[index++] = line[i++];
            }
            expression[index] = '\0';
            sheet[row][col].expression = expression;
            col++;
        }
        row++;
    }
    fclose(fp);
}

void EXPORT(char *file_name, Cell **sheet) {
    FILE *fp = fopen(file_name, "w");

    for (int i = 0; i < MAXSIZE; i++) {
        for (int j = 0; j < MAXSIZE; j++) {
            fprintf(fp, "%d", sheet[i][j].data);
            if (sheet[i][j].expression != NULL) {
                fprintf(fp, "%c", ':');
                fprintf(fp, "%s", sheet[i][j].expression);
            }
            if (j != MAXSIZE - 1)
                fprintf(fp, "%c", ',');
        }
        fprintf(fp, "%c", '\n');
    }

    fclose(fp);
}

int main() {
    Cell **sheet = create_sheet();
    char *prev_used_file = NULL;

    while (1) {
        printf(">");
        char *input = (char *)malloc(sizeof(char) * 20);
        gets(input);

        Command *command = parse_command(input);

        if (command -> type == 0)
            break;

        if (command -> type == 1)
            printf("%d\n", GET(command -> cell, sheet));

        else if (command -> type == 2)
            SET(command, sheet);

        else if (command -> type == 3)
            PRINT(sheet);

        else if(command -> type == 4) {
            IMPORT(command, sheet);
            prev_used_file = command -> file_name;
        }

        else if (command -> type == 5) {
            EXPORT(command -> file_name, sheet);
            prev_used_file = command -> file_name;
        }

        else if (command -> type == 6)
            if (prev_used_file != NULL)
                EXPORT(prev_used_file, sheet);
            else
                printf("No file to save\n");
        else
            printf("Invalid command\n");

    }

    return 0;
}