
#ifndef BLOCK_FILE_SYSTEM_COMMANDS_H
#define BLOCK_FILE_SYSTEM_COMMANDS_H

#include "structures.h"

int format_required(Metadata metadata , char *);

void format(char *);

Metadata copyfrom(char *file_name, char *disk, Metadata metadata);

void copyto(char *output_filename, char *input_filename, Metadata metadata,char *disk);

void ls(Metadata metadata);

Metadata delete_file(char *file_name, Metadata metadata, char *disk);

#endif //BLOCK_FILE_SYSTEM_COMMANDS_H
