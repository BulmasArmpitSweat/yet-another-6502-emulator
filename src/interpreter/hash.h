#ifndef DJFDK_E3JD_dEJKEJF_JDKK
#define DJFDK_E3JD_dEJKEJF_JDKK

#include "../include/6502-types.h"

#define HASH_TABLE_SIZE 256

typedef struct {
    AddressingModes mode;
    unsigned char opcode;
} Instruction_bytecode_info;

typedef struct {
    char mnemonic[4];  // 3-letter mnemonic plus null terminator
    Instruction_bytecode_info info[30];  // Array to hold different modes and their opcodes (adjust size as needed)
    int info_count;  // Number of valid entries in the info array
} MnemonicInfo;

typedef struct HashNode {
    MnemonicInfo mnemonicInfo;
    struct HashNode* next;  // For chaining in case of collisions
} HashNode;

typedef struct {
    HashNode* buckets[HASH_TABLE_SIZE];
} HashMap;

unsigned int hash_function(const char* mnemonic);
void init_hash_map(HashMap* map);
void insert_mnemonic(HashMap* map, const char* mnemonic, InstructionInfo* info, int info_count);
MnemonicInfo* lookup_mnemonic(HashMap* map, const char* mnemonic);

#endif /* DJFDK_E3JD_dEJKEJF_JDKK */