/*Funcion de hashing djb2. Fuente: http://www.cse.yorku.ca/~oz/especialidades.html*/
unsigned long f_hash(const char *str){
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}