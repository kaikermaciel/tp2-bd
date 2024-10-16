#ifndef ARTIGO_HPP
#define ARTIGO_HPP

#include <iostream>
#include <string>

class Artigo {
private:
    int id;
    std::string titulo;
    int ano;
    std::string autores;
    int citacoes;
    std::string atualizacao;
    std::string snippet;

public:
    Artigo() : id(0), ano(0), citacoes(0) {}

    // Setters
    void setId(int id) { this->id = id; }
    void setTitulo(const std::string& titulo) { this->titulo = titulo; }
    void setAno(int ano) { this->ano = ano; }
    void setAutores(const std::string& autores) { this->autores = autores; }
    void setCitacoes(int citacoes) { this->citacoes = citacoes; }
    void setAtualizacao(const std::string& atualizacao) { this->atualizacao = atualizacao; }
    void setSnippet(const std::string& snippet) { this->snippet = snippet; }

    // Getters
    int getId() const { return id; }
    std::string getTitulo() const { return titulo; }
    int getAno() const { return ano; }
    std::string getAutores() const { return autores; }
    int getCitacoes() const { return citacoes; }
    std::string getAtualizacao() const { return atualizacao; }
    std::string getSnippet() const { return snippet; }

    // Método para exibir os dados do artigo
    void exibir() const {
        std::cout << "ID: " << id << "\n"
                  << "Título: " << titulo << "\n"
                  << "Ano: " << ano << "\n"
                  << "Autores: " << autores << "\n"
                  << "Citações: " << citacoes << "\n"
                  << "Atualização: " << atualizacao << "\n"
                  << "Snippet: " << snippet << "\n\n";
    }
};

#endif // ARTIGO_HPP
