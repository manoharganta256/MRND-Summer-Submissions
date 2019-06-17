
#ifndef COMPILER_STRUCTURES_H
#define COMPILER_STRUCTURES_H

#define SYMBOLTABLESIZE 5
#define REGISTERSIZE 8
#define INTERMEDIATELENGHT 50
#define MEMORYSIZE 256
#define BLOCKSIZE 10
#define STACKSIZE 10

struct line {
    int opcode;
//    int variable_code;
    char variable;
    int len;
    int register_code1;
    int register_code2;
    int register_code3;
    int operator_opcode;
    char *lable;
};

struct symboltable {
    char name[2];
    int address;
    int size;
};

struct intermediatetable {
    int instruction_number;
    int opcode;
    int parameters[5];
};

struct blocktable {
    char name[2];
    int instruction_number;
};


typedef struct line Line;
typedef struct symboltable SymbolTable;
typedef struct intermediatetable IntermediateTable;
typedef struct blocktable BlockTable;

#endif //COMPILER_STRUCTURES_H
