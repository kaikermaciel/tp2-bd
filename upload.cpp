#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "hashTable.hpp" 
#include "parsing.cpp"
#include "primario.hpp"
#include "secundario.hpp"

using namespace std;

int main(int argc, char *argv[]){
	if(argc<2) {
        cout << "Qual o nome do arquivo de entrada?\nEx: upload <file>\n";
        return 1;
    }

    ifstream file(argv[1]);
    string line;

    if (!file){
        printf("Não foi possível encontrar o arquivo.\n");
        exit(1);
    }

    fstream *hashFile = new fstream(HASH_FILE,fstream::in|fstream::out|fstream::trunc|ios::binary); //Cria arquivo de dados
    initialize_output_file(hashFile);                       // Aloca o arquivo de dados

    fstream *secondaryIndexFile = new fstream(SECONDARY_INDEX_FILE,fstream::in|fstream::out|fstream::trunc|ios::binary); //Cria arquivo de indice secundário
    fstream *primaryIndexFile = new fstream(INDEX_FILE,fstream::in|fstream::out|fstream::trunc|ios::binary); //Cria arquivo de indice primário

    cout << "Carregando os registros no arquivo de dados."<<endl;

    while (getline(file, line)){
        Article article = parsing(line); 
        insert_hash(hashFile, article); 	
    }

    cout << "Carregando os dados no arquivo de índice primário..." << endl;
    insert_PrimaryIndex_file(hashFile, primaryIndexFile);

    cout << "Carregando os dados no arquivo de índice secundário..." << endl;
    insert_SecondaryIndex_file(hashFile, secondaryIndexFile); //Carrega os dados no arquivo de índice secundário

    cout << "COMEMORA JOGADOR! TUDO CERTO TUDO TIGRE " << endl;
    hashFile->close();
    primaryIndexFile->close();
    secondaryIndexFile->close();
    
}
