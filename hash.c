#include "testing.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#define CORRECTOR 

#define FACTOR_DE_CARGA 0.7
#define TAM_INICIAL 23

// ******************************************************************
//                        PROGRAMA PRINCIPAL


typedef void (*hash_destruir_dato_t)(void *);

enum campo_estado{
    VACIO,
    OCUPADO,
    BORRADO,
};

typedef struct campo_hash{
    char* clave;
    void* dato;
    enum campo_estado estado;
}campo_hash_t;

typedef struct hash{
    campo_hash_t* tabla;
    size_t tam;
    size_t cant;
    hash_destruir_dato_t destruir_dato; 
}hash_t;

campo_hash_t *crear_tabla_hash(size_t capacidad){
    campo_hash_t *tabla = malloc(sizeof(campo_hash_t)*capacidad);
	if(!(tabla)) return NULL;
    return tabla;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if(!(hash)) return NULL;
    hash->tabla = crear_tabla_hash(TAM_INICIAL);
    if(!(hash->tabla)){
        free(hash);
        return NULL;
    }
    size_t pos = 0;
    hash->tam = TAM_INICIAL;
    while(pos < hash->tam){
        hash->tabla[pos].estado = VACIO;
        pos++;
    }
    hash->cant =  0;
    hash->destruir_dato = destruir_dato;
    return hash;
}

bool redimensionar(hash_t *hash, size_t capacidad){

    campo_hash_t *tabla_ant = hash->tabla;
    size_t tam_ant = hash->tam; 
    hash->tam = capacidad;
    hash->tabla = crear_tabla_hash(capacidad);
    
    if (hash->tabla == NULL) return false;

    size_t pos = 0;
	
    hash->cant = 0;
    while(pos < hash->tam){
        hash->tabla[pos].clave = NULL;
		hash->tabla[pos].dato = NULL;
        hash->tabla[pos].estado = VACIO;
        pos++;
    }

    bool redimension_ok = true;

    for (size_t i = 0; i < tam_ant; i++) {
        if(tabla_ant[i].estado == OCUPADO){
            void *dato = tabla_ant[i].dato;
            void *clave = tabla_ant[i].clave;
            redimension_ok = hash_guardar(hash, clave, dato);
            free(clave);
        }
    }

    free(tabla_ant);
    return redimension_ok;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    if ((hash->cant)*100 / hash->tam >= FACTOR_DE_CARGA * 100 ){
        if(!redimensionar(hash, hash->tam*2)) return false;
    }
    char* clave_copia = strdup(clave);
    if(!(clave_copia)) return false;
    unsigned long pos = f_hash(clave_copia) % hash->tam;
    for (size_t i = 0; i < hash->tam; i++){
        if (hash->tabla[pos].estado != OCUPADO && !(hash_pertenece(hash, clave))){
            hash->tabla[pos].clave = clave_copia;
            hash->tabla[pos].dato = dato;
            hash->tabla[pos].estado = OCUPADO;
            hash->cant++;
            return true;
        }else if (hash->tabla[pos].estado == OCUPADO && strcmp(hash->tabla[pos].clave,clave_copia) == 0){
            hash_destruir_dato_t destruir_dato = hash->destruir_dato;
            if (destruir_dato){
                destruir_dato((hash->tabla[pos].dato));
            }
            hash->tabla[pos].dato = dato;
            free(clave_copia);
            return true;
        }
        pos = (pos + 5 * i) % hash->tam;       
    }
    return false;
}

void *hash_borrar(hash_t *hash, const char *clave){
    unsigned long pos = f_hash(clave) % hash->tam;
    if(hash->tabla[pos].estado == VACIO) return NULL;
    for (size_t i = 0; i < hash->tam; i++){
        pos = (pos + 5 * i) % hash->tam;
        if (hash->tabla[pos].estado == OCUPADO && strcmp(hash->tabla[pos].clave,clave) == 0){
            free(hash->tabla[pos].clave);
            void* dato_borrado = hash->tabla[pos].dato;
            hash->tabla[pos].dato = NULL;
            hash->tabla[pos].estado = BORRADO;
            hash->cant--;
            return dato_borrado;
        } else if (hash->tabla[pos].estado == VACIO){
            return NULL;
        }
    }        
    return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    unsigned long pos = f_hash(clave) % hash->tam;
    if(!hash_pertenece(hash, clave)) return NULL;
    for (size_t i = 0; i < hash->tam; i++){
        pos = (pos + 5 * i) % hash->tam;
        if (hash->tabla[pos].estado == OCUPADO && strcmp(hash->tabla[pos].clave,clave) == 0){
            return hash->tabla[pos].dato;
        }
        else if(hash->tabla[pos].estado  == VACIO){
            return NULL;
        }
    }    
    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    unsigned long pos = f_hash(clave) % hash->tam;
    for (size_t i = 0; i < hash->tam; i++){
        pos = (pos + 5 * i) % hash->tam;
        if (hash->tabla[pos].estado == OCUPADO && strcmp(hash->tabla[pos].clave,clave) == 0){
            return true;
        }
        else if(hash->tabla[pos].estado  == VACIO){
            return false;
        }
    } 
    return false;
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cant;
}

void hash_destruir(hash_t *hash){
    hash_destruir_dato_t destruir_dato = hash->destruir_dato;
    size_t pos = 0;
    while(pos < hash->tam){
        if (hash->tabla[pos].estado == OCUPADO){
            if(destruir_dato != NULL){ 
                destruir_dato(hash->tabla[pos].dato); 
            }
            free(hash->tabla[pos].clave);
        }
        pos++;
    }
    free(hash->tabla);
    free(hash);
}

typedef struct hash_iter{
    size_t pos;
    const hash_t* hash;
} hash_iter_t;

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!(iter)) return NULL;
    iter->hash = hash;
    size_t pos = 0;
    while (pos < hash->tam && hash->tabla[pos].estado != OCUPADO){
        pos++;
    }
    iter->pos = pos;
    return iter;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    if (iter->pos != iter->hash->tam){
        return false;
    }
    return true; 
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if (hash_iter_al_final(iter)) return false;
    iter->pos++; 
    while (!(hash_iter_al_final(iter))){
        if (iter->hash->tabla[iter->pos].estado == OCUPADO) return true;
        iter->pos++;
    }
    return true;
}


const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if (iter->pos != iter->hash->tam){
        return iter->hash->tabla[iter->pos].clave;
    }
    return NULL;
}

//void hash_iter_destruir(hash_iter_t *iter){
//    free(iter);