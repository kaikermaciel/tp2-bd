#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Artigo.hpp"


int main(int argc, char const *argv[]) {
    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Deu pra abrir o arquivo " << argv[1] << " não ó negão" << std::endl;
        return 1;       
    }
    
    std::string linha;
    std::vector<Artigo> artigos;

    while (std::getline(file, linha)) {
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

            // Adiciona o artigo à lista
            artigos.push_back(artigo);
        }
    }

    // Exibe os artigos lidos
    for (const auto& art : artigos) {
        art.exibir();
    }

    return 0;
}
