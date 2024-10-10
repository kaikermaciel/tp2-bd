#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


int main(int argc, char const *argv[]){

    std::ifstream file(argv[1]);

    if (!file.is_open()){
        std::cerr << "Deu pra abrir o arquivo " << argv[1] << " não ó negão" << std::endl;
        return 1;    
    }
    
    std::string linha;

    while(std::getline(file, linha)){
        std::stringstream ss(linha);

        std::string valor;
        std::vector<std::string> colunas;

        while (std::getline(ss, valor, ';'))
        {
            colunas.push_back(valor);
        }
        
        for (size_t i = 0; i < colunas.size(); ++i){
            std::cout << colunas[i];
            if (i < colunas.size() - 1){
                std::cout << " | ";
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }
    return 0;
}
