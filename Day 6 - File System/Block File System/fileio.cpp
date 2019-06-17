
#include <stdio.h>
#include "fileio.h"
#include "structures.h"

void read_block(char *buffer, int block_number, char *disk) {
    FILE *harddisk = fopen(disk, "rb+");

    if (!harddisk) {
        printf("fileio: Cannot open harddisk\n");
        return;
    }

    int block_offset = block_number * BLOCKSIZE;

    fseek(harddisk, block_offset, SEEK_SET);

    fread(buffer, BLOCKSIZE, 1, harddisk);

    fclose(harddisk);
}

void write_block(char *buffer, int block_number, char *disk, int last_block) {
    FILE *harddisk = fopen(disk, "rb+");

    if (!harddisk) {
        printf("fileio: Cannot open harddisk\n");
        return;
    }

    int block_offset = block_number * BLOCKSIZE;

    fseek(harddisk, block_offset, SEEK_SET);

    int size = last_block != 0 ? last_block : BLOCKSIZE;

    fwrite(buffer, size, 1, harddisk);

    fclose(harddisk);
}
