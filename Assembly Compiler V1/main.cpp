#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "structures.h"
#include "parser.h"
#include "instructions.h"

SymbolTable **create_symboltable() {
    SymbolTable **pSymbolTable = (SymbolTable **)malloc(sizeof(SymbolTable *) * SYMBOLTABLESIZE);

    for (int i = 0; i < SYMBOLTABLESIZE; i++)
        pSymbolTable[i] = (SymbolTable *)malloc(sizeof(SymbolTable));

    return pSymbolTable;
}

IntermediateTable **create_intermediatetable() {
    IntermediateTable **pIntermediateTable = (IntermediateTable **)malloc(sizeof(IntermediateTable *) * INTERMEDIATELENGHT);

    for (int i = 0; i < INTERMEDIATELENGHT; i++)
        pIntermediateTable[i] = (IntermediateTable *)malloc(sizeof(IntermediateTable));

    return pIntermediateTable;
}

BlockTable **create_blocktable() {
    BlockTable **pBlockTable = (BlockTable **)malloc(sizeof(BlockTable *) * BLOCKSIZE);

    for (int i = 0; i < BLOCKSIZE; i++)
        pBlockTable[i] = (BlockTable *)malloc(sizeof(BlockTable));

    return pBlockTable;
}

void dump_to_file(char *output_file, IntermediateTable **pIntermediateTable, int *len) {
    FILE *pFile = fopen(output_file, "w");

    for(int i = 0; i < *len; i++) {
        int opcode = pIntermediateTable[i] -> opcode;
        fprintf(pFile, "%d", opcode);
        fprintf(pFile, "%c", ' ');
        if (opcode == 1 || opcode == 2) {
            fprintf(pFile, "%d", pIntermediateTable[i] -> parameters[0]);
            fprintf(pFile, "%c", ' ');
            fprintf(pFile, "%d", pIntermediateTable[i] -> parameters[1]);
        }

        else if (opcode == 3 || opcode == 4 || opcode == 5) {
            for (int j = 0; j < 3; j++) {
                fprintf(pFile, "%d", pIntermediateTable[i] -> parameters[j]);
                if (j != 2)
                    fprintf(pFile, "%c", ' ');
            }
        }
        else if (opcode == 7) {
            for (int j = 0; j < 4; j++) {
                fprintf(pFile, "%d", pIntermediateTable[i] -> parameters[j]);
                if (j != 3)
                    fprintf(pFile, "%c", ' ');
            }
        }
        else if (opcode == 13 || opcode == 14 || opcode == 6)
            fprintf(pFile, "%d", pIntermediateTable[i] -> parameters[0]);

        fprintf(pFile, "%c", '\n');
    }

    fclose(pFile);
}

int main() {
    char input_file[] = "input.txt";

    FILE *input_fp = fopen(input_file, "r");
    char buffer[256];
    int declarations = 1, end = 0;

    SymbolTable **pSymbolTable = create_symboltable();
    int symbol_table_position = 0;

    int *memory = (int *)malloc(sizeof(int) * MEMORYSIZE);
    int current_memory_position = 0;

    IntermediateTable **pIntermediateTable = create_intermediatetable();
    int intermediate_table_position = 0;

    int *registers = (int *)malloc(sizeof(int) * REGISTERSIZE);
    fillArray(registers, REGISTERSIZE, 0);

    BlockTable **pBlockTable = create_blocktable();
    int block_table_position = 0;

    int *stack = (int *)malloc(sizeof(int) * STACKSIZE);
    int top = -1;

    while (fgets(buffer, 256, input_fp) && !end) {
        int len = strlen_(buffer);
        buffer[len] = '\0';

        if (declarations) {
            if(str_compare(buffer, (char *)"START:")) {
                declarations = 0;
                continue;
            }

            int data = (buffer[0] == 'C') ? 0 : 1;
            int index = data ? 5 : 6;
            int size = 1;

            pSymbolTable[symbol_table_position] -> name [0] = buffer[index];
            pSymbolTable[symbol_table_position] -> name [1] = '\0';

            if (data) {
                if (buffer[index + 1]) {
                    size = 0;
                    index += 2;
                    while (buffer[index] != ']') {
                        size *= 10;
                        size += (int) (buffer[index] - '0');
                        index++;
                    }
                }
            }
            else {
                size = 0;
                int val = 0;
                index++;
                while (buffer[index] == ' ' || buffer[index] == '=')
                    index++;

                while (buffer[index]) {
                    val *= 10;
                    val += (int) (buffer[index] - '0');
                    index++;
                }

                memory[current_memory_position] = val;
            }

            pSymbolTable[symbol_table_position] -> address = current_memory_position;
            pSymbolTable[symbol_table_position] -> size = size;

            symbol_table_position++;
            if (data)
                current_memory_position += size;
            else
                current_memory_position++;
        }

        else {
            Line *line = parse_line(buffer);

            switch (line -> opcode) {
                case 0:
                    pBlockTable[block_table_position] -> name[0] = buffer[0];
                    pBlockTable[block_table_position] -> name[1] = '\0';

                    pBlockTable[block_table_position] -> instruction_number = intermediate_table_position + 1;

                    block_table_position++;
                    break;

                case 1:
                case 2:
                    MOV(line -> opcode, line -> variable, line -> len, line -> register_code1, pSymbolTable, &symbol_table_position, pIntermediateTable, &intermediate_table_position);
                    break;

                case 3:
                case 4:
                case 5:
                    CALC(line -> opcode, line -> register_code1, line -> register_code2, line -> register_code3, pIntermediateTable, &intermediate_table_position);
                    break;

                case 6:
                    if (line -> lable)
                        JUMP(line -> lable, pIntermediateTable, &intermediate_table_position, pBlockTable, &block_table_position);
                    else {
                        ELSE(pIntermediateTable, &intermediate_table_position);
                        stack[++top] = intermediate_table_position;
                    }
                    break;

                case 7:
                    IF(line -> opcode ,line -> register_code1, line -> register_code2, line -> operator_opcode, pIntermediateTable, &intermediate_table_position, registers);
                    stack[++top] = intermediate_table_position;
                    break;

                case 13:
                    PRINT(line -> opcode, line -> register_code1, pIntermediateTable, &intermediate_table_position);
                    break;

                case 14:
                    READ(line -> opcode, line -> register_code1 ,pIntermediateTable, &intermediate_table_position);
                    break;
                case 15:
                    ENDIF(pIntermediateTable, &intermediate_table_position, stack, &top);
                    break;

                case 16:
                    end = 1;
                default:
                    break;
            }

        }

    }

    char output_file[] = "output.txt";
    dump_to_file(output_file ,pIntermediateTable, &intermediate_table_position);

    free(memory);

    return 0;
}