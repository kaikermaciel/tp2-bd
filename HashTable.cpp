#include <iostream>
#include <fstream>
#include <cstring>

#define CAPACITY 500000 // Tamanho da tabela hash.

unsigned long hash_function(const char *str) {
    unsigned long i = 0;
    for (int j = 0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

// Define o item da tabela hash.
struct Ht_item {
    char *key;
    char *value;
};

// Define a tabela hash.
struct HashTable {
    Ht_item **items;
    int size;
    int count;
};

// Função para criar um novo item da tabela hash.
Ht_item *create_item(const char *key, const char *value) {
    Ht_item *item = (Ht_item *)malloc(sizeof(Ht_item));
    item->key = (char *)malloc(strlen(key) + 1);
    item->value = (char *)malloc(strlen(value) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

// Função para criar a tabela hash.
HashTable *create_table(int size) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item **)calloc(table->size, sizeof(Ht_item *));
    return table;
}

// Função para inserir na tabela hash e gravar em arquivo.
void ht_insert(HashTable *table, const char *key, const char *value, std::ofstream &file) {
    Ht_item *item = create_item(key, value);
    unsigned long index = hash_function(key);

    if (table->items[index] == nullptr) {
        // Se a chave não existe.
        table->items[index] = item;
        table->count++;

        // Gravar no arquivo binário
        file.write(reinterpret_cast<const char*>(&table->size), sizeof(int));
        file.write(reinterpret_cast<const char*>(&table->count), sizeof(int));

        int key_length = strlen(item->key);
        file.write(reinterpret_cast<const char*>(&key_length), sizeof(int));
        file.write(item->key, key_length);

        int value_length = strlen(item->value);
        file.write(reinterpret_cast<const char*>(&value_length), sizeof(int));
        file.write(item->value, value_length);
    } else {
        // Se a chave já existe, atualiza o valor.
        if (strcmp(table->items[index]->key, key) == 0) {
            strcpy(table->items[index]->value, value);
        } else {
            std::cout << "Collision occurred for key: " << key << "\n";
        }
    }
}

// Função para liberar memória.
void free_table(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        Ht_item *item = table->items[i];
        if (item != nullptr) {
            free(item->key);
            free(item->value);
            free(item);
        }
    }
    free(table->items);
    free(table);
}

HashTable *ht_read_from_file(const char *filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for reading.\n";
        return nullptr;
    }

    // Cria uma nova tabela hash.
    HashTable *table = create_table(CAPACITY);

    // Lê o tamanho da tabela e a contagem de itens.
    file.read(reinterpret_cast<char*>(&table->size), sizeof(int));
    file.read(reinterpret_cast<char*>(&table->count), sizeof(int));

    // Lê os itens da tabela hash.
    for (int i = 0; i < table->size; i++) {
        int key_length;
        file.read(reinterpret_cast<char*>(&key_length), sizeof(int));

        if (key_length != -1) { // -1 indica que o item não existe
            char *key = (char *)malloc(key_length + 1);
            file.read(key, key_length);
            key[key_length] = '\0'; // Adiciona terminador nulo

            int value_length;
            file.read(reinterpret_cast<char*>(&value_length), sizeof(int));
            char *value = (char *)malloc(value_length + 1);
            file.read(value, value_length);
            value[value_length] = '\0'; // Adiciona terminador nulo

            // Insere o item na tabela hash
            ht_insert(table, key, value, file);

            // Libera a memória temporária
            free(key);
            free(value);
        }
    }

    file.close();
    return table;
}
