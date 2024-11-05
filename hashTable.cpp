#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include "hashTable.hpp"

int collision = 0;

unsigned long hash_function(int id) {
    return id % NUMBER_OF_BUCKETS;
}

void initialize_output_file(std::fstream *output_file){
    Block buffer = {0};
    std::cout << "Alocando arquivo de dados " << std::endl;

    // Preenche os buckets com blocos vazios
    for(int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        output_file->write((char*)&buffer, sizeof(Block));
    }
}

void print_article(Article artigo) {
    std::cout << "--------------------GREAT SUCESS! YOU'VE FOUNDED THE ARTICLE :) ----------------------------" 
              << "\n*ID: " << artigo.id
              << "\n*Titulo: " << artigo.titulo 
              << "\n*Ano: " << artigo.ano
              << "\n*Autor: " << artigo.autores
              << "\n*Citações:" << artigo.citacoes
              << "\n*Atualização: " << artigo.atualizacao
              << "\n*Snippet: " << artigo.snippet 
              << std::endl << std::endl;
}

Block bucket_by_id(std::fstream *file_hash, int id){
    Block buffer = {0};
    int key = hash_function(id);

    file_hash->seekg(key * sizeof(Block), std::ios::beg); 
    file_hash->read((char*)&buffer, sizeof(Block));

    return buffer; 
}

bool insert_overflow(Article artigo) {
    std::fstream overflow_file(OVERFLOW_FILE, std::ios::app | std::ios::binary);
    if (!overflow_file) {
        std::cerr << "Erro ao abrir o arquivo de overflow!" << std::endl;
        return false;
    }
    overflow_file.write((char*)&artigo, sizeof(Article));
    overflow_file.close();
    return true;
}

bool insert_hash(std::fstream *file_hash, Article artigo){
    Block buffer = bucket_by_id(file_hash, artigo.id);
    Article *artigos_em_bloco = (Article*)&buffer.bloco; 
    int i;
    
    if (buffer.quantidade_registers < NUMBER_OF_REGISTERS) {
        // Inserção normal no bucket principal
        for (i = 0; i < buffer.quantidade_registers; i++) {
            if (artigo.id < artigos_em_bloco[i].id) {
                for (int j = buffer.quantidade_registers; j > i; j--) {
                    memcpy(&artigos_em_bloco[j], (char*)&artigos_em_bloco[i - 1], sizeof(Article));
                }
                break;
            }
        }
        memcpy(&artigos_em_bloco[i], (char*)&artigo, sizeof(Article));
        buffer.quantidade_registers++;
        file_hash->seekp(-sizeof(Block), std::ios::cur);
        file_hash->write((char*)&buffer, sizeof(Block));
        return true;
    } else {
        // Inserção no arquivo de overflow
        collision++;
        return insert_overflow(artigo);
    }
}

Article search_in_overflow(int id) {
    std::fstream overflow_file(OVERFLOW_FILE, std::ios::in | std::ios::binary);
    if (!overflow_file) {
        std::cerr << "Erro ao abrir o arquivo de overflow!" << std::endl;
        return {0}; // Retorna um artigo vazio se houver erro
    }
    
    Article artigo;
    while (overflow_file.read((char*)&artigo, sizeof(Article))) {
        if (artigo.id == id) {
            overflow_file.close();
            return artigo; // Retorna o artigo se encontrado
        }
    }
    overflow_file.close();
    return {0}; // Retorna um artigo vazio se não for encontrado
}

Article search_by_id(std::fstream *file_hash, int id) {
    Block buffer = bucket_by_id(file_hash, id);
    Article *artigos_em_bloco = (Article*)&buffer.bloco;

    int start = 0;
    int finish = buffer.quantidade_registers - 1;
    while (start <= finish) {
        int i = (start + finish) / 2;
        if (artigos_em_bloco[i].id == id) {
            print_article(artigos_em_bloco[i]);
            return artigos_em_bloco[i];
        } else if (artigos_em_bloco[i].id < id) {
            start = i + 1;
        } else {
            finish = i - 1;
        }
    }

    // Busca no arquivo de overflow se o artigo não foi encontrado
    Article overflow_result = search_in_overflow(id);
    if (overflow_result.id != 0) { // Verifica se encontrou o artigo no overflow
        print_article(overflow_result);
        return overflow_result;
    }

    std::cout << "Mission failed! We'll get them next time...\n";
    return {0};
}

Block bucket_by_position(std::fstream *file_hash, int posicao){
    Block buffer = {0};
    file_hash->seekg(posicao * sizeof(Block), std::ios::beg);
    file_hash->read((char*)&buffer, sizeof(Block));
    return buffer;
}

Article article_by_title(std::fstream *file_hash, int posicao, char titulo_busca[SIZE_TITLE]){
    Block buffer = bucket_by_position(file_hash, posicao);
    Article *artigos_em_bloco = (Article*)&buffer.bloco;

    for (int i = 0; i < buffer.quantidade_registers; i++) {
        if (strcmp(titulo_busca, artigos_em_bloco[i].titulo) == 0) {
            print_article(artigos_em_bloco[i]);
            return artigos_em_bloco[i];
        }
    }

    std::cout << "Mission failed! We'll get them next time...\n";
    return {0};
}
