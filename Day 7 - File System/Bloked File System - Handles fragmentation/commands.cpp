
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

int get_blocknumber(const char *occupied) {
    int current = 1;

    while (current < BLOCKSIZE) {
        if (occupied[current] == '0')
            return current;
        current++;
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

    for (int i = 0; i < BLOCKS; i++) {
        write_block(buffer, i, disk, 0);
    }

    Metadata m;
    m.magic_number = MAGICNUMBER;
    m.valid_files = 0;

    m.occupied[0] = '1';
    for (int i = 1; i < BLOCKS; i++) {
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

    int file_index = metadata.valid_files;
    int len = file_length(input_file);
    int required = (len / BLOCKSIZE) + 1;

    int number = get_blocknumber(metadata.occupied);

    if (number == -1) {
        printf("Not enough space");
        return metadata;
    }

    if (required == 1) {
        fread(block, BLOCKSIZE, 1, input_file_ptr);
        write_block(block, number, disk, len);
        fclose(input_file_ptr);
        metadata.table[file_index].block_number = number;
        metadata.occupied[number] = '1';
    }
    else {
        char page[BLOCKSIZE] = {0};
        int index = 0;
        int indirect_block = number;
        metadata.occupied[indirect_block] = '1';

        number = get_blocknumber(metadata.occupied);

        while (fread(block, BLOCKSIZE, 1, input_file_ptr)) {
            write_block(block, number, disk, 0);
            metadata.occupied[number] = '1';
            while (number > 0) {
                page[index++] = (char)(number % 10 + '0');
                number /= 10;
            }
            page[index++] = ',';
            number = get_blocknumber(metadata.occupied);
        }
        write_block(block, number, disk, len % BLOCKSIZE);
        metadata.occupied[number] = '1';
        while (number > 0) {
            page[index++] = (char)(number % 10 + '0');
            number /= 10;
        }

        page[index] = ',';
        metadata.table[file_index].block_number = indirect_block;
//        write_block(page, indirect_block, disk, required);
        write_block(page, indirect_block, disk, index);
    }


    int index = 0;
    while (input_file[index]) {
        metadata.table[file_index].name[index] = input_file[index];
        index++;
    }

    metadata.table[file_index].name[index] = '\0';

    metadata.table[file_index].number_of_blocks = (unsigned)required;
    metadata.table[file_index].file_length = (unsigned)len;

    metadata.valid_files += 1;

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

    int len = metadata.table[file_index].file_length;

    len = len % BLOCKSIZE;

    char *buffer = (char *)malloc(sizeof(char) * BLOCKSIZE);

    FILE *output_file_ptr = fopen(output_filename, "wb");

    if (metadata.table[file_index].number_of_blocks == 1) {
        read_block(buffer, metadata.table[file_index].block_number, disk);
        fwrite(buffer, len, 1, output_file_ptr);
    }

    else {
        char *page = (char *)malloc(sizeof(char) * BLOCKSIZE);

        int indirect_block = metadata.table[file_index].block_number;

        read_block(page, indirect_block, disk);

        int index = 0;

        for (int i = 0; i < metadata.table[file_index].number_of_blocks; i++) {
            int num = 0;
            while (page[index] != ',' && page[index]) {
                num *= 10;
                num += (int)(page[index++] - '0');
            }

            index++;
            read_block(buffer, num, disk);
            if (i == metadata.table[file_index].number_of_blocks - 1)
                fwrite(buffer, len, 1, output_file_ptr);
            else
                fwrite(buffer, BLOCKSIZE, 1, output_file_ptr);
        }

        free(page);
    }


    free(buffer);
    fclose(output_file_ptr);

}

void ls(Metadata metadata) {
    if (metadata.valid_files == 0)
        return;

    int file_index = 0;

    for (; file_index < metadata.valid_files; file_index++) {
        if (metadata.occupied[file_index] == '1')
            printf("Name: %s, Size: %d\n", metadata.table[file_index].name, metadata.table[file_index].number_of_blocks);
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

    char *block = (char *)malloc(sizeof(char) * BLOCKSIZE);

    if (metadata.table[file_index].number_of_blocks == 1) {
        metadata.occupied[metadata.table[file_index].block_number] = '0';
    }
    else {
        read_block(block, metadata.table[file_index].block_number, disk);
        metadata.occupied[metadata.table[file_index].block_number] = '0';

        int index = 0;
        for (int i = 0; i < metadata.table[file_index].number_of_blocks; i++) {

            int num = 0;
            while (block[index] != ',' && block[index]) {
                num *= 10;
                num += (int)(block[index++] - '0');
            }

            index++;
            metadata.occupied[num] = '0';
        }
    }

    if (file_index == metadata.valid_files - 1)
        metadata.valid_files -= 1;
    else
        metadata.table[file_index] = metadata.table[metadata.valid_files - 1];

    memcpy(block, &metadata, sizeof(Metadata));
    write_block(block, 0, disk, 0);
    free(block);

    return metadata;

}
