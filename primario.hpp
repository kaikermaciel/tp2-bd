#ifndef INDICE_PRIMARIO_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include "hashTable.hpp"

#define INDEX_FILE "primary_indexFile"

#define TREE_ORDER 1000
#define POINTERS_QUANTITY 2 * TREE_ORDER
#define KEY_QUANTITY POINTERS_QUANTITY - 1

typedef struct PrimaryNode{
    int size;
    int position;
    int key[2*TREE_ORDER];
    int pointer[2*TREE_ORDER+1];
} PrimaryNode;


typedef struct AuxiliarNode{
    int key;
    int leftNode;
    int rightNode;
} AuxiliarNode;

typedef struct FatherNode{
    int position;
    int nodeQuantity;
} FatherNode;

void insert_PrimaryIndex_file(std::fstream *hashFile, std::fstream *primaryIndexFile);
void seek1(const char *data_path, const char *index_path, int chave);



#endif