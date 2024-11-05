#include <iostream>
#include "hashTable.hpp"

int main(int argc, char *argv[])
{
    if(argc<2) {
        std::cout << "Digite o ID.\nEx: findrec <ID>\n";
        return 1;
    }

    std::fstream *hash_file = new std::fstream(HASH_FILE,std::fstream::in|std::ios::binary);

    if (hash_file->is_open()) {
        search_by_id(hash_file,atoi(argv[1]));   
    }
    else {
        std::cout << "Erro ao abrir o arquivo\n";
    }
    return 0;
}