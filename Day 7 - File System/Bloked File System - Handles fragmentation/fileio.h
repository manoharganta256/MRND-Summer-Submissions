
#ifndef BLOCK_FILE_SYSTEM_FILEIO_H
#define BLOCK_FILE_SYSTEM_FILEIO_H

void read_block(char *buffer, int block_number, char *dick);
void write_block(char *buffer, int block_number, char *disk, int last_block);

#endif //BLOCK_FILE_SYSTEM_FILEIO_H
