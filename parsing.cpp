#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "hashTable.hpp"

void copy(char* copy_to, std::string copy_by, int size) {
    strncpy(copy_to, copy_by.c_str(), size);
    copy_to[size-1] = 0;
}

Article parsing(std::string line){
	Article article;
	std::string delimiter = "\";"; 
	size_t position = 0;
	std::string token;
	int column = 0;

	while ((position = line.find(delimiter)) != std::string::npos) {
       
       token = line.substr(1, position-1);
       //Extracao dos campos
	    if (column == 0){
	    	article.id= atoi(token.c_str());
	    }
	    else if(column == 1){
	    	copy(article.titulo, token.c_str(), SIZE_TITLE);
	    }
	    else if(column == 2){
	    	article.ano = atoi(token.c_str());
	    }
	    else if(column == 3){

            copy(article.autores, token.c_str(), SIZE_AUTHORS);
	    }
	    else if(column == 4){
	    	article.citacoes = atoi(token.c_str());
	    }
        else if(column == 5){
            copy(article.atualizacao, token.c_str(), SIZE_UPDATE);
        }
	    line.erase(0, position + delimiter.length());   
	    column ++;
	}

	if(line.length() > 0){
        token = line.substr(0, line.length()-1);

        if(strcmp(token.c_str(), "NULL") != 0){
            token = line.substr(1, line.length()-3); 
        }
        copy(article.snippet, token.c_str(), SIZE_SNIPPET);
	}
    return article;
}