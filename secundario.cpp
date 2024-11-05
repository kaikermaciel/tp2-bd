#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

#include "secundario.hpp"
#include "hashTable.hpp"

SecondaryFatherNode *father_node = NULL;
std::fstream *hash_file, *secondaryIndex_file;

void update_fatherIndex_secondary(){
    secondaryIndex_file->seekp(0, secondaryIndex_file->beg);
    secondaryIndex_file->write((char*)father_node, sizeof(SecondaryFatherNode));
}

void initialize_fatherNode_secondary(){
    father_node = (SecondaryFatherNode*)malloc(sizeof(SecondaryFatherNode));
    father_node->rootPosition = -1;
    father_node->nodeQuantity = 0;
    update_fatherIndex_secondary();
}

Node* create_new_secondaryNode(){
    Node *new_node = (Node*)malloc(sizeof(Node));
    
    new_node->size = 0;
    new_node->position = 0;
    
    for(int i = 0; i < SECONDARY_KEY_QUANTITY - 1; i++){
        strcpy(new_node->key[i], "\0");
        new_node->pointer[i] = 0;
    }

    new_node->pointer[SECONDARY_POINTERS_QUANTITY  - 1] = 0;
    return new_node;
}

int insert_node_indexFile(Node *node_to_insert){
    father_node->nodeQuantity++;
    
    secondaryIndex_file->seekp(father_node->nodeQuantity*sizeof(Node), secondaryIndex_file->beg);
    secondaryIndex_file->write((char*)node_to_insert, sizeof(Node));

    update_fatherIndex_secondary();

    return father_node->nodeQuantity;
}

void update_node_into_file(Node *node_to_update){
    secondaryIndex_file->seekp(node_to_update->position*sizeof(Node), secondaryIndex_file->beg);
    secondaryIndex_file->write((char*)node_to_update, sizeof(Node));
}

void insert_key_into_avaiablePage(Node *node, char key[SIZE_TITLE], int pointer){
    int i;

    for(i = 0; i < node->size; i++){
        if(strcmp(node->key[i], key) > 0){
            for(int j = node->size; j > i; j--){
                strcpy(node->key[j], node->key[j-1]);
                node->pointer[j] = node->pointer[j-1];
            }
            break;
        }
    }
    strcpy(node->key[i], key);
    node->pointer[i] = pointer;
    node->size++;
    update_node_into_file(node);
}

void insert_key_into_avaiableNode(Node *node, char key[SIZE_TITLE], int pointer){
    int i;
    for(i = 0; i < node->size; i++){
        if(strcmp(node->key[i], key) > 0 ){
            for(int j = node->size; j > i; j--){
                strcpy(node->key[j], node->key[j-1]);
                node->pointer[j+1] = node->pointer[j];
            }
            break;
        }
    }
    strcpy(node->key[i], key);
    node->pointer[i+1] = pointer * (-1);
    node->size++;
    update_node_into_file(node);
}

NodeAux* insert_key_into_fullPage(Node *node, char key[SIZE_TITLE], int pointer){
    
    bool inserted = false;
    char pivot[SIZE_TITLE];

    Node *new_node = create_new_secondaryNode();
    NodeAux *new_parent = (NodeAux*)malloc(sizeof(NodeAux));

    strcpy(pivot, node->key[SECONDARY_TREE_ORDER]);

    if (strcmp(key, pivot) < 0){
        int i, j = 0;
        for(i = SECONDARY_TREE_ORDER; i <= SECONDARY_KEY_QUANTITY - 1; i++){
            strcpy(new_node->key[j], node->key[i]);
            new_node->pointer[j] = node->pointer[i];
            new_node->size++;
            node->pointer[i] = 0;
            strcpy(node->key[i], "\0");
            node->size--;
            j++;
        }
        for(i = SECONDARY_TREE_ORDER - 1; i >= 0; i--){
            if(strcmp(key, node->key[i]) > 0){ break; }
            else{
                strcpy(node->key[i+1], node->key[i]);
                node->pointer[i+1] = node->pointer[i];
            }
        }
        strcpy(node->key[i+1], key);
        node->pointer[i+1] = pointer;
        node->size++;
    }
    else{
        int i,j=0;
		for(i=SECONDARY_TREE_ORDER; i<=SECONDARY_KEY_QUANTITY - 1; i++){
			if(inserted == false && strcmp(key,node->key[i]) < 0){
				strcpy(new_node->key[j],key);
				new_node->pointer[j] = pointer;
				new_node->size++;
				j++;
				inserted = true;
			}
			strcpy(new_node->key[j],node->key[i]);
			new_node->pointer[j] = node->pointer[i];
			new_node->size++;
			strcpy(node->key[i],"\0");
			node->pointer[i] = 0;
			node->size--;
			j++;
		}
		if(inserted == false){
			strcpy(new_node->key[j],key);
			new_node->pointer[j] = pointer;
			new_node->size++;
		}
    }
    new_node->pointer[SECONDARY_POINTERS_QUANTITY - 1] = node->pointer[SECONDARY_POINTERS_QUANTITY - 1];
	new_node->position = insert_node_indexFile(new_node);
	node->pointer[SECONDARY_POINTERS_QUANTITY - 1] = new_node->position * (-1);
	update_node_into_file(node);
	update_node_into_file(new_node);
	strcpy(new_parent->key, pivot);
	new_parent->leftPointer = node->position * (-1);
	new_parent->rightPointer =  new_node->position * (-1);
	free(new_node);
	return new_parent;
}

NodeAux* insert_key_into_fullNode(Node *node, char key[SIZE_TITLE], int pointer){
	
    bool inserted = false;
    char pivot[SIZE_TITLE];
	
    Node *new_node = create_new_secondaryNode();
	NodeAux *new_parent = (NodeAux *) malloc(sizeof(NodeAux));
	
    strcpy(pivot,node->key[SECONDARY_TREE_ORDER]);
	strcpy(node->key[SECONDARY_TREE_ORDER],"\0");

	node->size--;

	if(strcmp(key,pivot) < 0){
		int i,j=0;
		new_node->pointer[0] = node->pointer[SECONDARY_TREE_ORDER+1];
		for(i=SECONDARY_TREE_ORDER+1; i<=SECONDARY_KEY_QUANTITY - 1; i++){
			strcpy(new_node->key[j],node->key[i]);
			new_node->pointer[j+1] = node->pointer[i+1];
			new_node->size++;
			strcpy(node->key[i],"\0");
			node->pointer[i+1] = 0;
			node->size--;
			j++;
		}
		for(i=SECONDARY_TREE_ORDER; i>0; i--){
			if(strcmp(key,node->key[i]) > 0){
				break;
			}else{
				strcpy(node->key[i],node->key[i-1]);
				node->pointer[i+1] = node->pointer[i];
			}
		}
		strcpy(node->key[i],key);
		node->pointer[i+1] = -1 * pointer;
		node->size++;
	}

	else{
		int i,j=0;
		new_node->pointer[0] = node->pointer[SECONDARY_TREE_ORDER+1];
		for(i=SECONDARY_TREE_ORDER+1; i<=SECONDARY_KEY_QUANTITY - 1; i++){
			if(inserted == false && strcmp(key,node->key[i]) < 0){
				strcpy(new_node->key[j],key);
				new_node->pointer[j+1] = -1 * pointer;
				new_node->size++;
				j++;
				inserted = true;
			}
			strcpy(new_node->key[j],node->key[i]);
			new_node->pointer[j+1] = node->pointer[i+1];
			new_node->size++;
			node->pointer[i+1] = 0;
			strcpy(node->key[i],"\0");
			node->size--;
			j++;
		}
		if(inserted == false){
			strcpy(new_node->key[j], key);
			new_node->pointer[j+1] = -1 * pointer;
			new_node->size++;
		}
	}

	new_node->position = insert_node_indexFile(new_node);
	update_node_into_file(new_node);
	update_node_into_file(node);

	strcpy(new_parent->key, pivot);
	new_parent->leftPointer = node->position * (-1);
	new_parent->rightPointer = new_node->position * (-1);
	free(new_node);
	return new_parent;
}

Node* read_secondaryNode_on_file(int position){
	//Retorna o nó do arquivo de indice a partir da posição informada
	if(position > 0){
		std::cout << "Endereço errado. Insira novamente!" << std::endl;
		return NULL;
	}

	Node *node = create_new_secondaryNode();
	//Posiciona o cursor na posição
	secondaryIndex_file->seekg(position * (-1) * sizeof(Node),secondaryIndex_file->beg);
	//Copia o nó
	secondaryIndex_file->read((char *)node, sizeof(Node));
	return node;
}

NodeAux* insert_key_into_tree(Node *node, char key[SIZE_TITLE], int pointer){

	NodeAux *index = NULL;

	if(node->pointer[0] < 0){
		int position;
		int minimum_position = 0;
		int maximum_position = node->size-1;

		//Encontra a posição do pointer
		while(minimum_position <= maximum_position){
			position = (minimum_position + maximum_position) / 2;
			if(strcmp(key,node->key[position]) < 0){
				maximum_position = position - 1;
			}
			else{
				minimum_position = position + 1;
			}
		}
		if(maximum_position < 0){
			maximum_position = 0;
		}
		else if(strcmp(key,node->key[maximum_position]) >= 0){
			maximum_position++;
		}
		position = maximum_position;

		index = insert_key_into_tree(read_secondaryNode_on_file(node->pointer[position]),key,pointer);

		if(index!= NULL){
			//Caso a página tenha espaço
			if(strcmp(node->key[SECONDARY_KEY_QUANTITY - 1], "\0") == 0){
				insert_key_into_avaiableNode(node, index->key, index->rightPointer * (-1));
				free(index);
				index = NULL;
			}
			else{
				NodeAux *return_node = insert_key_into_fullNode(node, index->key, index->rightPointer * (-1));
				free(index);
				index = return_node;
			}
		}
	}
	else{
		if(strcmp(node->key[SECONDARY_KEY_QUANTITY - 1],"\0") == 0){
			insert_key_into_avaiablePage(node,key,pointer);
		}
		else{
			index = insert_key_into_fullPage(node,key,pointer);
		}
	}
	free(node);
	return index;
}

void populate_indexFile(){
	Block buffer = {0};
	int position;

	hash_file->clear();
	hash_file->seekg(0,hash_file->beg);

	for (int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        hash_file->read((char*)&buffer,sizeof(Block));
        if(buffer.quantidade_registers > 0) {
            Article *article_vector = (Article*)&buffer.bloco;
			//Percorre os registros do bloco
            for(int j=0; j<buffer.quantidade_registers; j++){
				NodeAux *page = insert_key_into_tree(read_secondaryNode_on_file(father_node->rootPosition),article_vector[j].titulo,i);

				if(page != NULL){
					Node *node = create_new_secondaryNode();
					//O novo nó recebe as informações do nó criado na árvore
					strcpy(node->key[0],page->key);
					node->pointer[0] = page->leftPointer;
					node->pointer[1] = page->rightPointer;
					node->size++;
					//Insere o nó no arquivo
					int position = insert_node_indexFile(node);
					node->position = position;
					update_node_into_file(node);
					//Atualiza posição da raiz
					father_node->rootPosition = position * (-1);
					update_fatherIndex_secondary();

					free(node);
					free(page);
				}
			}
        }
    }
}

void insert_SecondaryIndex_file(std::fstream *hashFile, std::fstream *secondary_indexFile){
	hash_file =  hashFile;
	secondaryIndex_file = secondary_indexFile;
	initialize_fatherNode_secondary();
	Node *root = create_new_secondaryNode();
	root->position = insert_node_indexFile(root); 	//Escreve nó raiz no arquivo						
	update_node_into_file(root);
	free(root);
	populate_indexFile();
}

void read_fatherNode_on_file(){
	if(father_node == NULL){
		father_node = (SecondaryFatherNode *)malloc(sizeof(father_node));
	}

	secondaryIndex_file->seekg(0,secondaryIndex_file->beg);
	secondaryIndex_file->read((char *)father_node, sizeof(father_node));
}

int seek2(std::fstream *data_path,std::fstream *index_path, char key[SIZE_TITLE]){
	hash_file = data_path;
	secondaryIndex_file = index_path;

	int position, minimum_position, maximum_position, pointer_position, i, blocks =0;
	Node *node;
	read_fatherNode_on_file();
	
	position = father_node->rootPosition;

	while(1){
		node = read_secondaryNode_on_file(position);
		blocks++;
		if(node != NULL){
			if(node->pointer[0] < 0){
				minimum_position = 0;
				maximum_position=node->size-1;
				while(minimum_position <= maximum_position){
					pointer_position = (minimum_position + maximum_position) / 2;
					if(strcmp(key,node->key[pointer_position]) < 0){
						maximum_position = pointer_position - 1;
					}
					else{
						minimum_position = pointer_position + 1;
					}
				}
				if(maximum_position < 0){
					maximum_position = 0;
				}
				else if(strcmp(key, node->key[maximum_position]) >= 0){
					maximum_position++;
				}
				pointer_position = maximum_position;
				position = node->pointer[pointer_position];
			}else{
				break;
			}
		}
		else{
			std::cout << "ops.. não deu pra carregar o nó" << std::endl;
			std::cout << "Registro " << key << " nao encontrado" << std::endl;
			return -1;
		}
	}

	minimum_position = 0;
	maximum_position = node->size-1;

	while(minimum_position <= maximum_position){
		pointer_position = (minimum_position + maximum_position) / 2;
		if(strcmp(key,node->key[pointer_position]) == 0){
			break;
		}else if(strcmp(key,node->key[pointer_position]) < 0){
			maximum_position = pointer_position - 1;
		}else{
			minimum_position = pointer_position + 1;
		}
	}

	article_by_title(hash_file, node->pointer[pointer_position], key);
	std::cout<< "------------------------------------------------" << std::endl;
	std::cout << std::endl << "Total de blocos de índice armazenados: " << father_node->nodeQuantity << std::endl;
	std::cout << "Quantidade total de blocos lidos: " << blocks << std::endl;
	secondaryIndex_file->close();
	hash_file->close();
	free(secondaryIndex_file);
	free(hash_file);
	free(father_node);
	return -1;
}