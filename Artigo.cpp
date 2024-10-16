#include "Artigo.hpp"
#include <iostream>

// Construtor
Artigo::Artigo() : id(0), ano(0), citacoes(0) {}

// Setters
void Artigo::setId(int id) { this->id = id; }
void Artigo::setTitulo(const std::string& titulo) { this->titulo = titulo; }
void Artigo::setAno(int ano) { this->ano = ano; }
void Artigo::setAutores(const std::string& autores) { this->autores = autores; }
void Artigo::setCitacoes(int citacoes) { this->citacoes = citacoes; }
void Artigo::setAtualizacao(const std::string& atualizacao) { this->atualizacao = atualizacao; }
void Artigo::setSnippet(const std::string& snippet) { this->snippet = snippet; }

// Getters
int Artigo::getId() const { return id; }
std::string Artigo::getTitulo() const { return titulo; }
int Artigo::getAno() const { return ano; }
std::string Artigo::getAutores() const { return autores; }
int Artigo::getCitacoes() const { return citacoes; }
std::string Artigo::getAtualizacao() const { return atualizacao; }
std::string Artigo::getSnippet() const { return snippet; }

// Método para exibir os dados do artigo
void Artigo::exibir() const {
    std::cout << "ID: " << id << "\n";
    std::cout << "Título: " << titulo << "\n";
    std::cout << "Ano: " << ano << "\n";
    std::cout << "Autores: " << autores << "\n";
    std::cout << "Citações: " << citacoes << "\n";
    std::cout << "Atualização: " << atualizacao << "\n";
    std::cout << "Snippet: " << snippet << "\n\n";
}
