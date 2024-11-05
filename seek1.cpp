#include "primario.hpp"



int main(int argc, char *argv[]){
    seek1(HASH_FILE, INDEX_FILE, atoi(argv[1]));
    return 0;
}

