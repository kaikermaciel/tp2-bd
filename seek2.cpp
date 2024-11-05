#include <iostream>
#include <string>
#include "secundario.hpp"
#include "hashTable.hpp"



int main(int argc, char *argv[]){
	if(argc != 2){
		std::cout << "Qual o título do registro que deseja procurar?.\nEx: seek2 <Titulo>\n" << std::endl;
		return 1;
	}

    std::fstream *hash_file = new std::fstream(HASH_FILE,std::fstream::in|std::ios::binary);
    std::fstream *secondaryIndex_file = new std::fstream(SECONDARY_INDEX_FILE,std::fstream::in|std::ios::binary);

    if (hash_file->is_open() && secondaryIndex_file->is_open()) {
        seek2(hash_file,secondaryIndex_file, argv[1]);   
    }
    else {
        std::cout << "Não foi possível abrir o arquivo.\n";
    }
    return 0;
	
}