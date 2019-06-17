#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "commands.h"
#include "fileio.h"

int main() {
    char input[30];
    char disk[] = "harddisk.hdd";

    Metadata metadata;

    char *buffer = (char *)malloc(sizeof(char) * BLOCKSIZE);
    read_block(buffer, 0, disk);
    memcpy(&metadata, buffer, sizeof(Metadata));
    free(buffer);

    while (1) {
        printf(">>");

        if (format_required(metadata, disk)) {
            printf("format required!(y/n): ");
            gets(input);
            if (input[0] == 'y') {
                format(disk);
                printf(">>");
            }
            else {
                printf("Format terminated\n");
                break;
            }
        }

        gets(input);

        Command *command = parse_command(input);

        if (command -> type == -1)
            break;

        if (command -> type == 0) {
            printf("Not a valid command");
            continue;
        }

        if (command -> type == 1) {
            metadata =  copyfrom(command -> file_name, disk, metadata);
        }

        if (command -> type == 2) {
            copyto(command -> file_name, command -> file_name2, metadata, disk);
        }

        if (command -> type == 3) {
            ls(metadata);
        }

        if (command -> type == 4) {
            metadata = delete_file(command -> file_name, metadata, disk);
        }

        free(command -> file_name);
        free(command);
    }

    return 0;
}