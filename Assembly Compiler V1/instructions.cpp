
#include "instructions.h"
#include "util.h"

int get_address(char variable, SymbolTable **pSymbolTable, int *len) {
    for (int i = 0; i < *len; i++) {
        if (pSymbolTable[i]->name[0] == variable)
            return pSymbolTable[i] -> address;
    }

    return -1;
}

void READ(int opcode, int register1, IntermediateTable **pIntermediateTable, int *intermediate_table_position) {
    pIntermediateTable[*intermediate_table_position] -> instruction_number = *intermediate_table_position + 1;
    pIntermediateTable[*intermediate_table_position] -> opcode = opcode;
    pIntermediateTable[*intermediate_table_position] -> parameters[0] = register1;

    *intermediate_table_position += 1;

}

void PRINT(int opcode, int register1, IntermediateTable **pIntermediateTable, int *intermediate_table_position) {
    pIntermediateTable[*intermediate_table_position] -> instruction_number = *intermediate_table_position + 1;
    pIntermediateTable[*intermediate_table_position] -> opcode = opcode;
    pIntermediateTable[*intermediate_table_position] -> parameters[0] = register1;

    *intermediate_table_position += 1;

}

void CALC(int opcode ,int dest_register, int register1, int register2, IntermediateTable **pIntermediateTable, int *intermediate_table_position) {
    pIntermediateTable[*intermediate_table_position] -> instruction_number = *intermediate_table_position + 1;
    pIntermediateTable[*intermediate_table_position] -> opcode = opcode;
    pIntermediateTable[*intermediate_table_position] -> parameters[0] = dest_register;
    pIntermediateTable[*intermediate_table_position] -> parameters[1] = register1;
    pIntermediateTable[*intermediate_table_position] -> parameters[2] = register2;

    *intermediate_table_position += 1;
}

void MOV(int opcode, char variable, int len, int register_code,SymbolTable **pSymbolTable, int *symbol_table_position, IntermediateTable **pIntermediateTable, int *intermediate_table_position) {
    pIntermediateTable[*intermediate_table_position] -> instruction_number = *intermediate_table_position + 1;
    pIntermediateTable[*intermediate_table_position] -> opcode = opcode;

    int address = get_address(variable, pSymbolTable, symbol_table_position);

    if (opcode == 1) {
        pIntermediateTable[*intermediate_table_position] -> parameters[0] = address + len;
        pIntermediateTable[*intermediate_table_position] -> parameters[1] = register_code;
    }
    else {
        pIntermediateTable[*intermediate_table_position] -> parameters[0] = register_code;
        pIntermediateTable[*intermediate_table_position] -> parameters[1] = address + len;
    }

    *intermediate_table_position += 1;

}

void IF(int opcode, int register1, int register2, int operation, IntermediateTable **pIntermediateTable,int *intermediate_table_position, int *registes) {
    pIntermediateTable[*intermediate_table_position] -> instruction_number = *intermediate_table_position + 1;
    pIntermediateTable[*intermediate_table_position] -> opcode = opcode;

    pIntermediateTable[*intermediate_table_position] -> parameters[0] = register1;
    pIntermediateTable[*intermediate_table_position] -> parameters[1] = register2;
    pIntermediateTable[*intermediate_table_position] -> parameters[2] = operation;
    pIntermediateTable[*intermediate_table_position] -> parameters[3] = -1;

    *intermediate_table_position += 1;
}

void JUMP(char *label, IntermediateTable **pIntermediateTable, int *intermediate_table_position, BlockTable **pBlockTable, int *block_table_position) {
    pIntermediateTable[*intermediate_table_position] -> instruction_number = *intermediate_table_position + 1;
    pIntermediateTable[*intermediate_table_position] -> opcode = 6;
    int index = 0;
    for (; index < *block_table_position; index++) {
        if (str_compare(label, pBlockTable[index] -> name))
            break;
    }

    pIntermediateTable[*intermediate_table_position] -> parameters[0] = pBlockTable[index]->instruction_number;

    *intermediate_table_position += 1;
}

void ELSE(IntermediateTable **pIntermediateTable, int *intermediate_table_position) {
    pIntermediateTable[*intermediate_table_position]->instruction_number = *intermediate_table_position + 1;
    pIntermediateTable[*intermediate_table_position]->opcode = 6;
    pIntermediateTable[*intermediate_table_position]->parameters[0] = -1;

    *intermediate_table_position += 1;
}

void ENDIF(IntermediateTable **pIntermediateTable, int *intermediate_table_position, int *stack, int *top) {
    int val = stack[*top];
    *top -= 1;

    int index = *intermediate_table_position - 1;

    while (pIntermediateTable[index--] -> instruction_number != val);

    index++;
    pIntermediateTable[index] -> parameters[0] = *intermediate_table_position + 1;

    int temp = val;
    val = stack[*top];
    *top -= 1;

    while (pIntermediateTable[index--] -> instruction_number != val);
    index++;
    pIntermediateTable[index] -> parameters[3] = temp + 1;

}
