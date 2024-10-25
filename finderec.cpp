#include <iostream>
#include "Artigo.hpp"
#define FILE_HASH "hash"

int main(int argc, char *argv[]){
    int id;
    std::cout << "Digite o ID de busca:\n";
    std::cin >> id;

    std::fstream *file_hash = new std::fstream(FILE_HASH, std::fstream::in|std::ios::binary);
    if (file_hash->is_open()){
        search_by_id(file_hash, id);
        return 1;
    }
    else{
        std::cout << "Não foi possível abrir arquivo.\n";
    }
    return 0;
}