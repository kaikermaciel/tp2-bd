#ifndef INDICE_SECUNDARIO_HPP

#include <fstream>
#include "hashTable.hpp"

#define SECONDARY_INDEX_FILE "secondary_indexFile"

#define SECONDARY_TREE_ORDER 200
#define SECONDARY_POINTERS_QUANTITY 2 * SECONDARY_TREE_ORDER + 1
#define SECONDARY_KEY_QUANTITY 2 * SECONDARY_TREE_ORDER

struct Node{
    int size;
    int position;
    char key[SECONDARY_KEY_QUANTITY][SIZE_TITLE];
    int pointer[SECONDARY_POINTERS_QUANTITY];
};

struct NodeAux{
    char key[SIZE_TITLE];
    int leftPointer;
    int rightPointer;
};

struct SecondaryFatherNode{
    int rootPosition;
    int nodeQuantity;
};

void insert_SecondaryIndex_file(std::fstream *hashFile, std::fstream *secondaryIndexFile);
int seek2(std::fstream *data_path, std::fstream *index_path, char key[SIZE_TITLE]);

#endif