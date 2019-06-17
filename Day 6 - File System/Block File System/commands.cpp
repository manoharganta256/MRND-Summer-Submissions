
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "commands.h"
#include "fileio.h"
#include "util.h"

int file_length(char *file_name) {
    FILE *input_file_ptr = fopen(file_name, "rb");
    fseek(input_file_ptr, 0, SEEK_END);
    int len = ftell(input_file_ptr);
    fclose(input_file_ptr);
    return len;
}

int get_blocknumber(char *occupied, int required) {
    int current = 0, len = 0;

    while (current < BLOCKS) {
        int occp = (occupied[current + len] - '0');
        if (occp) {
            current++;
            continue;
        }

        len++;

        if (len == required && len < BLOCKS)
            return current;
    }
    return -1;
}

int format_required(Metadata metadata, char *disk) {
    char *buffer = (char *)malloc(sizeof(char) * BLOCKSIZE);
    read_block(buffer, 0, disk);

    Metadata new_metadata;
    memcpy(&new_metadata, buffer, sizeof(Metadata));

    free(buffer);

    if (new_metadata.magic_number == MAGICNUMBER)
        return 0;
    return 1;
}

void format(char *disk) {
    char buffer[BLOCKSIZE] = {0};

    for (int i = 0; i < BLOCKS + 1; i++) {
        write_block(buffer, i, disk, 0);
    }

    Metadata m;
    m.magic_number = MAGICNUMBER;

    for (int i = 0; i < BLOCKS; i++) {
        m.occupied[i] = '0';
    }

    memcpy(buffer, &m, sizeof(Metadata));
    write_block(buffer, 0, disk, 0);
}


Metadata copyfrom(char *input_file, char *disk, Metadata metadata) {

    FILE *input_file_ptr = fopen(input_file, "rb");
    if (!input_file_ptr) {
        printf("Cannot open input file\n");
        return metadata;
    }

    char *block = (char *)malloc(sizeof(char) * BLOCKSIZE);

    int len = file_length(input_file);
    int required = (len / BLOCKSIZE) + 1;

    int number = get_blocknumber(metadata.occupied, required);

    if (number == -1) {
        printf("Not enough space");
        return metadata;
    }


    while (fread(block, BLOCKSIZE, 1, input_file_ptr)) {
        write_block(block, number + 1, disk, 0);
        number++;
    }

    write_block(block, number + 1, disk, len % BLOCKSIZE);
    fclose(input_file_ptr);

    metadata.valid_files += 1;

    for (int i = number; i < number + required; i++)
        metadata.occupied[i] = '1';

    int index = 0;
    while (input_file[index]) {
        metadata.table[number].name[index] = input_file[index];
        index++;
    }

    metadata.table[number].name[index] = '\0';

    metadata.table[number].start_block = (unsigned)number;
    metadata.table[number].number_of_blocks = (unsigned)required;
    metadata.table[number].file_length = (unsigned)len;

    memcpy(block, &metadata, sizeof(Metadata));
    write_block(block, 0, disk, 0);

    free(block);

    return metadata;
}

void copyto(char *output_filename, char *input_filename, Metadata metadata, char *disk) {
    int file_index = 0;

    for (; file_index < metadata.valid_files; file_index++) {
        if (str_compare(input_filename, metadata.table[file_index].name))
            break;
    }

    if (file_index == metadata.valid_files) {
        printf("File not found\n");
        return;
    }

    int start_block = metadata.table[file_index].start_block;
//    int count = metadata.table[file_index].number_of_blocks;

    int len = metadata.table[file_index].file_length;

    char *buffer = (char *)malloc(sizeof(char) * BLOCKSIZE);
//    int current_block_count = start_block;

    FILE *output_file_ptr = fopen(output_filename, "wb");

    while (len >= BLOCKSIZE) {
        read_block(buffer, start_block, disk);
        fwrite(buffer, BLOCKSIZE, 1, output_file_ptr);
        start_block++;
        len -= BLOCKSIZE;
    }

    if (len != 0) {
        read_block(buffer, start_block, disk);
        fwrite(buffer, len, 1, output_file_ptr);
    }


    fclose(output_file_ptr);

}

void ls(Metadata metadata) {
    int file_index = 0;

    for (; file_index < metadata.valid_files; file_index++) {
        if (metadata.occupied[file_index] == '1')
            printf("Name: %s, Start Block: %d, Size: %d\n", metadata.table[file_index].name, metadata.table[file_index].start_block, metadata.table[file_index].number_of_blocks);
    }
}

Metadata delete_file(char *file_name, Metadata metadata, char *disk) {

    int file_index = 0;

    for (; file_index < metadata.valid_files; file_index++) {
        if (str_compare(file_name, metadata.table[file_index].name))
            break;
    }

    if (file_index == metadata.valid_files) {
        printf("File not found\n");
        return metadata;
    }

    int number = metadata.table[file_index].start_block;
    int count = metadata.table[file_index].number_of_blocks;

    if (file_index == metadata.valid_files - 1)
        metadata.valid_files -= 1;
    else
        metadata.table[file_index] = metadata.table[metadata.valid_files - 1];

    for (int i = number; i < number + count; i++) {
        metadata.occupied[i] = '0';
    }

    char *block = (char *)malloc(sizeof(char) * BLOCKSIZE);
    memcpy(block, &metadata, sizeof(Metadata));
    write_block(block, 0, disk, 0);

    free(block);

    return metadata;

}
