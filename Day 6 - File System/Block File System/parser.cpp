
#include <stdlib.h>
#include "parser.h"
#include "util.h"

int type(char *token) {
    str_lower(token);
    if (str_compare(token, (char *)"exit"))
        return -1;

    if (str_compare(token, (char *)"copyfrom"))
        return 1;

    if (str_compare(token, (char *)"copyto"))
        return 2;

    if (str_compare(token, (char *)"ls"))
        return 3;

    if (str_compare(token, (char *)"delete"))
        return 4;

    return 0;
}

Command *parse_command(char *input) {

    Command *command = (Command *)malloc(sizeof(Command));

    char token[30];

    int index = 0;
    while (*input != ' ' && *input) {
        token[index++] = *input++;
    }

    token[index] = '\0';

    command -> type = type(token);

    while (*input == ' ' || *input == '\t')
        input++;

    if (!*input)
        return command;

    command -> file_name = (char *)malloc(sizeof(char) * 20);
    index = 0;

    while (*input && *input != ' ') {
        command -> file_name[index++] = *input++;
    }

    command -> file_name[index] = '\0';

    if (command -> type == 2) {
        input++;
        command -> file_name2 = (char *)malloc(sizeof(char) * 20);
        index = 0;

        while (*input) {
            command -> file_name2[index++] = *input++;
        }

        command -> file_name2[index] = '\0';
    }

    return command;

}
