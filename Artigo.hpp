#ifndef ARTIGO_HPP
#define ARTIGO_HPP

#include <iostream>
#include <string>
#include <fstream>

#define SIZE_TITLE 300
#define SIZE_AUTHORS 150
#define SIZE_UPDATE 20
#define SIZE_SNIPPET 1024

#define NUMBER_OF_BUCKETS 908201

#define NUMBER_OF_REGISTERS 2
#define SIZE_REGISTERS NUMBER_OF_REGISTERS * (12 + SIZE_TITLE + SIZE_SNIPPET + SIZE_AUTHORS + SIZE_UPDATE)


struct Article
{
    int id;
    int ano;
    int citacoes;
    char titulo[SIZE_TITLE];
    char atualizacao[SIZE_UPDATE];
    char autores[SIZE_AUTHORS];
    char snippet[SIZE_SNIPPET];
};

struct Block
{
    int quantidade_registers;
    char bloco[SIZE_REGISTERS];
};


unsigned long hash_function(int id);
void print_article(Article artigo);
Block bucket_by_id(std::fstream *file_hash, int id);
bool insert_hash(std::fstream *file_hash, Article artigo);
Article search_by_id(std::fstream *file_hash, int id);
Block bucket_by_position(std::fstream *file_hash, int posicao);
Article article_by_title(std::fstream *file_hash, int posicao, char titulo_busca[SIZE_TITLE]);
#endif 