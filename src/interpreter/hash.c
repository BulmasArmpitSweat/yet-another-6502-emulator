#include "hash.h"

unsigned int hash_function(const char* mnemonic) {
    unsigned int hash = 0;
    while (*mnemonic) {
        hash = (hash << 5) + *mnemonic++;
    }
    return hash % HASH_TABLE_SIZE;
}

void init_hash_map(HashMap* map) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        map->buckets[i] = NULL;
    }
}

void insert_mnemonic(HashMap* map, const char* mnemonic, InstructionInfo* info, int info_count) {
    unsigned int index = hash_function(mnemonic);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    
    strcpy(newNode->mnemonicInfo.mnemonic, mnemonic);
    memcpy(newNode->mnemonicInfo.info, info, info_count * sizeof(InstructionInfo));
    newNode->mnemonicInfo.info_count = info_count;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

MnemonicInfo* lookup_mnemonic(HashMap* map, const char* mnemonic) {
    unsigned int index = hash_function(mnemonic);
    HashNode* node = map->buckets[index];
    
    while (node) {
        if (strcmp(node->mnemonicInfo.mnemonic, mnemonic) == 0) {
            return &node->mnemonicInfo;
        }
        node = node->next;
    }
    return NULL;  // Not found
}
