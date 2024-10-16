#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Artigo.hpp"   // Inclui a definição da classe Artigo
#include "HashTable.cpp" // Inclui a implementação da tabela hash

void parse_and_insert(const char *filename, HashTable *table, std::ofstream &file) {
    std::ifstream infile(filename);
    std::string linha;

    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Ignorar o cabeçalho (se houver)
    std::getline(infile, linha);

    while (std::getline(infile, linha)) {
        Artigo artigo;
        std::string valor;
        bool dentroDeAspas = false;

        for (char ch : linha) {
            if (ch == '"') {
                // Alterna o estado de estar dentro ou fora de aspas
                dentroDeAspas = !dentroDeAspas;
            }

            if (ch == ';' && !dentroDeAspas) {
                // Se encontramos um delimitador e não estamos dentro de aspas, adicionamos o valor
                if (!valor.empty()) {
                    // Remove aspas do valor, se existirem
                    if (valor.front() == '"') valor.erase(0, 1);
                    if (valor.back() == '"') valor.erase(valor.size() - 1);

                    // Armazena o valor na struct correspondente
                    if (artigo.getId() == 0) { // ID
                        artigo.setId(std::stoi(valor));
                    } else if (artigo.getTitulo().empty()) { // Título
                        artigo.setTitulo(valor);
                    } else if (artigo.getAno() == 0) { // Ano
                        artigo.setAno(std::stoi(valor));
                    } else if (artigo.getAutores().empty()) { // Autores
                        artigo.setAutores(valor);
                    } else if (artigo.getCitacoes() == 0) { // Citações
                        artigo.setCitacoes(std::stoi(valor));
                    } else if (artigo.getAtualizacao().empty()) { // Atualização
                        artigo.setAtualizacao(valor);
                    } else if (artigo.getSnippet().empty()) { // Snippet
                        artigo.setSnippet(valor);
                    }

                    valor.clear(); // Limpa o valor para o próximo campo
                }
            } else {
                valor += ch; // Adiciona o caractere ao valor atual
            }
        }

        // Adiciona o último valor, se houver
        if (!valor.empty()) {
            if (valor.front() == '"') valor.erase(0, 1);
            if (valor.back() == '"') valor.erase(valor.size() - 1);
            artigo.setSnippet(valor); // O snippet é o último valor

            // Inserir na tabela hash
            ht_insert(table, std::to_string(artigo.getId()).c_str(), artigo.getTitulo().c_str(), file);
        }
    }

    infile.close();
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ofstream file("hash_table.dat", std::ios::binary); // Abre o arquivo para escrita binária.
    if (!file) {
        std::cerr << "Error opening file for writing.\n";
        return 1;
    }

    HashTable *ht = create_table(CAPACITY);

    // Insira os dados a partir do arquivo CSV
    parse_and_insert(argv[1], ht, file);

    file.close(); // Fecha o arquivo após todas as inserções.

    // Aqui você pode adicionar mais lógica para utilizar a tabela hash conforme necessário.

    free_table(ht); // Libere a memória da tabela hash.
    return 0;
}
