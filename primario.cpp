#include "primario.hpp"

FatherNode *fathe_node = NULL;
std::fstream *file_index, *file_data;



void update_fatherNode(){
    file_index->seekp(0, file_index->beg);
    file_index->write((char *) fathe_node, sizeof(FatherNode));
}

void initialize_fatherNode_primary(){
    fathe_node = (FatherNode *)malloc(sizeof(FatherNode));
    fathe_node->position = -2;
    fathe_node->nodeQuantity = 0;
    update_fatherNode();
}

PrimaryNode* create_newNode_primary(){
    PrimaryNode *node = (PrimaryNode *) malloc(sizeof(PrimaryNode));
    node->size = 0;
    node->size = 0;

    for(int i = 0; i < KEY_QUANTITY; i++){
        node->key[i] = 0;
        node->pointer[i] = 0;
    }
    node->pointer[POINTERS_QUANTITY] = 0;
    return node;
}

int insert_node_indexFile(PrimaryNode *block){
    fathe_node->nodeQuantity++;
    file_index->seekp(fathe_node->nodeQuantity*sizeof(PrimaryNode), file_index->beg);
    file_index->write((char *)block, sizeof(PrimaryNode));
    update_fatherNode();
    return fathe_node->nodeQuantity;
}

void update_node_indexFile(PrimaryNode *block){
    file_index->seekp(block->position*sizeof(PrimaryNode), file_index->beg);
    file_index->write((char *)block, sizeof(PrimaryNode));
}

void insert_key_into_avaiablePage(PrimaryNode *node, int key, int pointer){
    int i, j;
    for(i = 0; i < node->size; i++){
        if(node->key[i] > key){
            for(j = node->size; j > i; j--){
                node->key[j] = node->key[j-1];
                node->pointer[j] = node->pointer[j-1];
            }
            break;
        }
    }
    node->key[i] = key;
    node->pointer[i] = pointer;
    node->size++;
    update_node_indexFile(node);
}

void insert_key_into_avaiableNode(PrimaryNode *node, int key, int pointer){
    int i, j;
    for(i = 0; i < node->size; i++){
        if(node->key[i] > key){
            for(j = node->size; j > i; j--){
                node->key[j] = node->key[j-1];
                node->pointer[j+1] = node->pointer[j];
            }
            break;
        }
    }
    node->key[i] = key;
    node->pointer[i+1] = pointer * (-1);
    node->size++;
    update_node_indexFile(node);
}

AuxiliarNode* insert_key_fullPage(PrimaryNode *node, int key, int pointer){
    int i, j, pivot, inserted = 0;
    
    PrimaryNode *new_node = NULL;
    AuxiliarNode *father_node = NULL;
    
    new_node = create_newNode_primary();
    father_node = (AuxiliarNode *)malloc(sizeof(AuxiliarNode));

    pivot = node->key[TREE_ORDER];
    node->key[TREE_ORDER] = 0;
    node->size--;
    
    if(key > pivot){
        j = 0;
        new_node->pointer[0] = node->pointer[TREE_ORDER + 1];
        for(i = TREE_ORDER + 1; i <= KEY_QUANTITY; i++){
            if(inserted == 0 && key < node->key[i]){
                new_node->key[j] = key;
                new_node->pointer[j+1] = pointer * (-1);
                new_node->size++;
                
                j++;
                inserted = 1;
            }
            new_node->key[j] = node->key[i];
            new_node->pointer[j+1] = node->pointer[i+1];
            new_node->size++;
            
            node->key[i] = 0;
            node->pointer[i+1] = 0;
            node->size--;
            
            j++;
        }
        if(inserted == 0){
            new_node->key[j] = key;
            new_node->pointer[j+1] = pointer * (-1);
            new_node->size++;
        }
    }
    else{
        j = 0;
        new_node->pointer[0] = node->pointer[TREE_ORDER + 1];
        for(i = TREE_ORDER + 1; i <= KEY_QUANTITY; i++){
            
            new_node->key[j] = node->key[i];
            new_node->pointer[j +1] = node->pointer[i+1];
            new_node->size++;
            
            node->key[i] = 0;
            node->pointer[i + 1] = 0;
            node->size--;
            
            j++;
        }
        for(i = TREE_ORDER; i > 0; i--){
            if(key > node->key[i]){break;}
            else{
                node->key[i] = node->key[i-1];
                node->pointer[i+1] = node->pointer[i];
            }
        }
        node->key[i] = key;
        node->pointer[i+1] = pointer * (-1);
        node->size++;
    }
    new_node->position = insert_node_indexFile(new_node);
    
    update_node_indexFile(new_node);
    update_node_indexFile(node);
    
    father_node->key = pivot;
    father_node->leftNode = node->position * (-1);
    father_node->rightNode = new_node->position * (-1);
    
    return father_node;
}

AuxiliarNode* insert_key_fullNode(PrimaryNode *node, int key, int pointer){
    int i, j, pivot;
    bool inserted = false;
    
    PrimaryNode *new_node = NULL;
    AuxiliarNode *father_node = NULL;

    new_node = create_newNode_primary();
    father_node = (AuxiliarNode *)malloc(sizeof(AuxiliarNode));
    pivot = node->key[TREE_ORDER];
    if(key > pivot){
        j = 0;
        for(i = TREE_ORDER; i <= KEY_QUANTITY; i++){
            if(inserted == false && key < node->key[i]){
                new_node->key[j] = key;
                new_node->pointer[j] = pointer;
                new_node->size++;
                
                j++;
                
                inserted = true;
            }
            new_node->key[j] = node->key[i];
            new_node->pointer[j] = node->pointer[i];
            new_node->size++;

            node->key[i] = 0;
            node->pointer[i] = 0;
            node->size--;
            
            j++;
        }
        if(inserted == false){
            new_node->key[j] = key;
            new_node->pointer[j] = pointer;
            new_node->size++;
        }
    }
    new_node->pointer[POINTERS_QUANTITY] = node->pointer[POINTERS_QUANTITY];
    new_node->position = insert_node_indexFile(new_node);
    node->pointer[POINTERS_QUANTITY] = new_node->position * (-1);

    update_node_indexFile(node);
    update_node_indexFile(new_node);

    father_node->key = pivot;
    father_node->leftNode = node->position * (-1);
    father_node->rightNode = new_node->position * (-1);
    free(new_node);

    return father_node;
}

PrimaryNode* read_node_primary_file(int position){
    
    if (position > 0){
        std::cout << "Ops... Endereço errado, coloque novamente" << std::endl;
        return NULL;
    }

    PrimaryNode *node = create_newNode_primary();
    
    file_index->seekg(position*(-1) * sizeof(PrimaryNode), file_index->beg);
    file_index->read((char *)node, sizeof(PrimaryNode));
    
    return node;
}

AuxiliarNode* insert_key_into_tree(PrimaryNode *node, int key, int pointer){
    
    int i, j;
    AuxiliarNode *node_aux = NULL;

    if(node->pointer[0] < 0){
        int smaller = 0, larger= node->size-1, position;

        while (smaller <= larger){
            position = (smaller + larger) / 2;
            key < node->key[position] ? larger = position - 1 : smaller = position + 1;
        }

        if(larger < 0){ larger = 0; }
        else if( key >= node->key[larger]){ larger++; }

        position = larger;
        node_aux = insert_key_into_tree(read_node_primary_file(node->pointer[position]), key, pointer);
        
        if (node){
            if(node->key[KEY_QUANTITY] == 0){
                insert_key_into_avaiableNode(node, node_aux->key, node_aux->rightNode * (-1));
                free(node_aux);
                node_aux = NULL;
            }
            else{
                AuxiliarNode *new_aux = NULL;
                new_aux = insert_key_fullNode(node, node_aux->key, node_aux->rightNode * (-1));
                free(node_aux);
                node_aux = new_aux;
            }
        }
        
    }
    else{
        if(node->key[KEY_QUANTITY] == 0){ insert_key_into_avaiablePage(node, key, pointer); } 
        else { node_aux = insert_key_fullPage(node, key, pointer); }
    }
    free(node);
    return node_aux;
}

void insert_into_tree(int key, int pointer){
    
    AuxiliarNode *node_aux;
    node_aux = insert_key_into_tree(read_node_primary_file(fathe_node->position), key, pointer);
    
    if(node_aux){
        PrimaryNode *new_node;
        int position;

        new_node = create_newNode_primary();
        new_node->key[0] = node_aux->key;
        new_node->pointer[0] = node_aux->leftNode;
        new_node->pointer[1] = node_aux->rightNode;
        new_node->size++;

        position = insert_node_indexFile(new_node);
        new_node->position = position;
        update_node_indexFile(new_node);
        fathe_node->position = position * (-1);
        update_fatherNode();

        free(new_node);
        free(node_aux);
    }
}

void initialize_index_primary_file(){
    Block buffer = {0};

    file_data->clear();
    file_data->seekg(0, file_data->beg);

    for(int i = 0; i < NUMBER_OF_BUCKETS; i++){
        file_data->read((char *)&buffer, sizeof(Block));

        if(buffer.quantidade_registers > 0){
            Article *articles_vector;
            articles_vector = (Article*)&buffer.bloco;
            insert_into_tree(articles_vector[0].id, i);
        }
    }
}

void insert_PrimaryIndex_file(std::fstream *hash_file, std::fstream *primary_indexFile){
    file_index = primary_indexFile;
    file_data = hash_file;

    initialize_fatherNode_primary();
    PrimaryNode *root = create_newNode_primary();
    root->position = insert_node_indexFile(root);
    update_node_indexFile(root);
    free(root);
    initialize_index_primary_file();
}

void read_fatherNode_at_indexFile(){
    if(!fathe_node){
        fathe_node = (FatherNode*)malloc(sizeof(FatherNode));
    }
    file_index->seekg(0, file_index->beg);
    file_index->read((char*)fathe_node, sizeof(FatherNode));
}

void open_indexFile(const char *path_indexFile){
    file_index = new std::fstream(path_indexFile, std::fstream::in | std::fstream::out | std::ios::binary);
    if(!file_index){
        std::cout << "ops... não consegui o arquivo de indice" << std::endl;
    }
}

void open_dataFile(const char *path_dataFile){
    file_data = new std::fstream(path_dataFile, std::fstream::in | std::fstream::out | std::ios::binary);
    if(!file_data){
        std::cout << "ops... não consegui o arquivo de dados" << std::endl;
    }
}

int search_key_into_indexFile(const char *path_dataFile,const char *path_indexFile, int key){
	
    int position, smaller, larger, pointer_position, i, blocks_read=0;
	PrimaryNode *node,*aux;
	
    open_indexFile(path_indexFile);
	open_dataFile(path_dataFile);
	read_fatherNode_at_indexFile();
	
    blocks_read++;
	position = fathe_node->position;

	while(1){
		node = read_node_primary_file(position);
		blocks_read++;
		if(node != NULL){
			if(node->pointer[0] < 0){
				smaller=0;
				larger=node->size-1;
				while(smaller <= larger){
					pointer_position = (smaller + larger) / 2;
					(key < node->key[pointer_position]) ? larger = pointer_position - 1 : smaller = pointer_position + 1;
				}
				if(larger < 0){
					larger = 0;
				}else if(key >= node->key[larger]){
					larger++;
				}
				pointer_position = larger;
				position = node->pointer[pointer_position];
			}else{
				break;
			}
		}else{
			std::cout << "ops.. não foi possivel carregar o arquivo" << std::endl;
			return -1;
		}
	}
	std::cout << "Quantidade de Blocos lidos: " << blocks_read << std::endl;
	smaller = 0;
	larger = node->size-1;
	while(smaller <= larger){
		pointer_position = (smaller + larger) / 2;
		if(key == node->key[pointer_position]){
			return node->pointer[pointer_position];
		}else if(key < node->key[pointer_position]){
			larger = pointer_position - 2;
		}else{
			smaller = pointer_position + 2;
		}
	}
	file_index->close();
	file_data->close();
	free(file_index);
	free(file_data);
	return -1;
}

Block* read_block_on_dataFile(std::fstream *data_file, int position) {
    
    Block *buffer= NULL;
    buffer = (Block*) malloc(sizeof(Block));
    
    int i,j;

    data_file->seekg(position*sizeof(Block), std::ios::beg);
    data_file->read((char*)buffer,sizeof(Block));

    return buffer;
}

void copy_data(Article *data_copyTo, Article *data_copyBy){
    data_copyTo->id = data_copyBy->id;
    data_copyTo->ano = data_copyBy->ano;
    data_copyTo->citacoes = data_copyBy->citacoes;
    strcpy(data_copyTo->titulo, data_copyBy->titulo);
    strcpy(data_copyTo->atualizacao, data_copyBy->atualizacao);
    strcpy(data_copyTo->snippet, data_copyBy->snippet);
}

Article* read_data_by_id(std::fstream *data_file, int position, int id){
    Block *bucket;
    Article *article, *output_article = NULL;

    int larger, smaller, position_pointer;
    
    output_article = (Article*)malloc(sizeof(Article));
    bucket = read_block_on_dataFile(data_file, position);
    article = (Article*)&bucket->bloco;

    smaller = 0;
    larger = bucket->quantidade_registers - 1;

    while (smaller <= larger){
        position_pointer = (smaller + larger) / 2;

        if(id == article[position_pointer].id){ 
            copy_data(output_article, &article[position_pointer]); 
            break;
        } else if(id < article[position_pointer].id){ larger = position_pointer - 1; }
        else{ smaller = position_pointer + 1; }

    }
    free(bucket);
    return output_article;
}

void seek1(const char *path_dataFile, const char *path_indexFile, int key){
    int position = 0;
    Article *article;
    position = search_key_into_indexFile(path_dataFile, path_indexFile, key);

    if(position < 0 ){
        std::cout << "ops.. registro não foi achado " << std:: endl;
    }

    else{
        open_dataFile(path_dataFile);
        article = read_data_by_id(file_data, position, key);
        print_article(*article);
        free(article);
        file_data->close();
    }
}

