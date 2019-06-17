
#ifndef COMPILER_INSTRUCTIONS_H
#define COMPILER_INSTRUCTIONS_H

#include "structures.h"

void MOV(int opcode, char variable, int len, int register_code,SymbolTable **pSymbolTable, int *symbol_table_position, IntermediateTable **pIntermediateTable, int *intermediate_table_position);
void CALC(int opcode,int dest_register, int register1, int register2, IntermediateTable **pIntermediateTable, int *intermediate_table_position);
void JUMP(char *label, IntermediateTable **pIntermediateTable, int *intermediate_table_position, BlockTable **pBlockTable, int *block_table_position);
void ELSE(IntermediateTable **pIntermediateTable, int *intermediate_table_position);
void IF(int opcode ,int register1, int register2, int operation, IntermediateTable **pIntermediateTable,int *intermediate_table_position, int *registes);
void READ(int opcode, int register1, IntermediateTable **pIntermediateTable, int *intermediate_table_position);
void PRINT(int opcode, int register1, IntermediateTable **pIntermediateTable, int *intermediate_table_position);
void ENDIF(IntermediateTable **pIntermediateTable, int *intermediate_table_position, int *stack, int *top);

#endif //COMPILER_INSTRUCTIONS_H
