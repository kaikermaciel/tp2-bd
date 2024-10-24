#include <iostream>
#include <fstream>
#include <cstring>
#include <fstream>
#include "Artigo.hpp"


unsigned long hash_function(int id) {
    return id % NUMBER_OF_BUCKETS;
}

Block bucket_by_id(fstream *f, int id){
    Block buffer = {0};
    int key = hashing(id);

    f->seekg (key * sizeof(Block), ios::beg);
    f->read ((char*)&buffer, sizeof(Block));

    return buffer;
}