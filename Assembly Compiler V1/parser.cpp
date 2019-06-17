
#include <stdlib.h>
#include "parser.h"
#include "util.h"
#include "structures.h"

int opcode(char *token) {
    if (str_compare(token, (char *)"MOV"))
        return 1;

    if (str_compare(token, (char *)"ADD"))
        return 3;

    if (str_compare(token, (char *)"SUB"))
        return 4;

    if (str_compare(token, (char *)"MUL"))
        return 5;

    if (str_compare(token, (char *)"JUMP") || str_compare(token, (char *)"ELSE"))
        return 6;

    if (str_compare(token, (char *)"IF"))
        return 7;

    if (str_compare(token, (char *)"EQ"))
        return 8;

    if (str_compare(token, (char *)"LT"))
        return 9;

    if (str_compare(token, (char *)"GT"))
        return 10;

    if (str_compare(token, (char *)"LTEQ"))
        return 11;

    if (str_compare(token, (char *)"GTEQ"))
        return 12;

    if (str_compare(token, (char *)"PRINT"))
        return 13;

    if (str_compare(token, (char *)"READ"))
        return 14;

    if (str_compare(token, (char *)"ENDIF"))
        return 15;

    if (str_compare(token, (char *)"END"))
        return 16;

    return 0; // loop block
}


Line *parse_line(char *input) {
    Line *line = (Line *)malloc(sizeof(Line));
    line -> lable = NULL;
    int index = 0;
    char token[30];

    while (*input == ' ')
        input++;

    while (*input != ' ' && *input != ':' && *input)
        token[index++] = *input++;

    token[index] = '\0';

    line -> opcode = opcode(token);

    if (line -> opcode == 15 || line -> opcode == 16 || line -> opcode == 0)
        return line;


    if (line -> opcode == 6) {

        if (!*input)
            return line;

        line -> lable = (char *)malloc(sizeof(char) * 2);
        input++;
        index = 0;
        for (; token[index] != '\0'; index++)
            line -> lable[index] = *input++;
        line -> lable[index] = '\0';

        return line;
    }

    if (*input == ' ')
        input++; // space
    if (line -> opcode == 1) {
        line -> len = 0;
        if (input[1] == 'X') { // mem to register
            line -> opcode = 2;
            line -> register_code1 = (int)(*input - 'A');
            input += 2;
            while (!isalpha(*input))
                input++;
            line -> variable = *input;
            input++;
            if (*input == '[') {
                input++;
                line -> len = (int)(*input - '0');
            }
        }
        else {
            line -> variable = *input;
            input++;
            if (*input == '[') {
                input++;
                line -> len = (int)(*input - '0');
            }
            while (!isalpha(*input))
                input++;
            line -> register_code1 = (int)(*input - 'A');
        }
    }

    if (line -> opcode == 3 || line -> opcode == 4 || line -> opcode == 5) {
        line -> register_code1 = (int)(*input - 'A');
        input += 2;
        while (*input == ' ' || *input == ',')
            input++;
        line -> register_code2 = (int)(*input - 'A');
        input += 2;
        while (*input == ' ' || *input == ',')
            input++;
        line -> register_code3 = (int)(*input - 'A');
    }

    if (line -> opcode == 7) {
        line -> register_code1 = (int) (*input - 'A');
        input += 3;

        index = 0;
        while (*input != ' ')
            token[index++] = *input++;

        token[index] = '\0';

        line -> operator_opcode = opcode(token);

        input++;

        line -> register_code2 = (int) (*input - 'A');
    }

    if (line -> opcode == 13 || line -> opcode == 14) {
        line -> register_code1 = (int) (*input - 'A');
    }

    return line;
}

