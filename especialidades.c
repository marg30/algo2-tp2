#include "testing.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "especialidades.h"
#include "cola.h"
#include "heap.h"
#define CORRECTOR 

#define FACTOR_DE_CARGA 0.7
#define TAM_INICIAL 23
#define URGENCIA "urgencia"
#define REGULAR "regular"

// ******************************************************************
//                        PROGRAMA PRINCIPAL

/*Funcion de hashing djb2. Fuente: http://www.cse.yorku.ca/~oz/especialidades.html*/
unsigned long f_hash(const char *str){
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

typedef void (*especialidades_destruir_dato_t)(void *);

enum campo_estado{
    VACIO,
    OCUPADO,
    BORRADO,
};

typedef struct campo_hash{
    char* clave;
    cola_t* cola_urgente;
    heap_t* pacientes_espera;
    enum campo_estado estado;
}campo_especialidades_t;

typedef struct especialidades {
    campo_especialidades_t* tabla;
    size_t tam;
    size_t cant;
    especialidades_destruir_dato_t destruir_cola;
    especialidades_destruir_dato_t destruir_heap;
}especialidades_t;

campo_especialidades_t *crear_tabla_especialidades(size_t capacidad){
    campo_especialidades_t *tabla = malloc(sizeof(campo_especialidades_t)*capacidad);
	if(!(tabla)) return NULL;
    return tabla;
}

especialidades_t *especialidades_crear(especialidades_destruir_dato_t destruir_cola, especialidades_destruir_dato_t destruir_heap){
    especialidades_t* especialidades = malloc(sizeof(especialidades_t));
    if(!(especialidades)) return NULL;
    especialidades->tabla = crear_tabla_especialidades(TAM_INICIAL);
    if(!(especialidades->tabla)){
        free(especialidades);
        return NULL;
    }
    size_t pos = 0;
    especialidades->tam = TAM_INICIAL;
    while(pos < especialidades->tam){
        especialidades->tabla[pos].estado = VACIO;
        pos++;
    }
    especialidades->cant =  0;
    especialidades->destruir_cola = destruir_cola;
    especialidades->destruir_heap = destruir_heap;
    return especialidades;
}

bool especialidades_redimensionar(especialidades_t *especialidades, size_t capacidad){

    campo_especialidades_t *tabla_ant = especialidades->tabla;
    size_t tam_ant = especialidades->tam; 
    especialidades->tam = capacidad;
    especialidades->tabla = crear_tabla_especialidades(capacidad);
    
    if (especialidades->tabla == NULL) return false;

    size_t pos = 0;
	
    especialidades->cant = 0;
    while(pos < especialidades->tam){
        especialidades->tabla[pos].clave = NULL;
		especialidades->tabla[pos].cola_urgente = NULL;
        especialidades->tabla[pos].pacientes_espera = NULL;
        especialidades->tabla[pos].estado = VACIO;
        pos++;
    }

    bool redimension_ok = true;

    for (size_t i = 0; i < tam_ant; i++) {
        if(tabla_ant[i].estado == OCUPADO){
            cola_t *cola_urgente = tabla_ant[i].cola_urgente;
            heap_t *pacientes_espera = tabla_ant[i].pacientes_espera;
            void *clave = tabla_ant[i].clave;
            redimension_ok = especialidades_guardar(especialidades, clave, cola_urgente, pacientes_espera);
            free(clave);
        }
    }

    free(tabla_ant);
    return redimension_ok;
}

bool especialidades_guardar(especialidades_t *especialidades, const char *clave, cola_t *cola_urgente, heap_t *en_espera) {
    if ((especialidades->cant)*100 / especialidades->tam >= FACTOR_DE_CARGA * 100 ){
        if(!especialidades_redimensionar(especialidades, especialidades->tam*2)) return false;
    }
    char* clave_copia = strdup(clave);
    if(!(clave_copia)) return false;
    unsigned long pos = f_hash(clave_copia) % especialidades->tam;
    for (size_t i = 0; i < especialidades->tam; i++){
        if (especialidades->tabla[pos].estado != OCUPADO && !(especialidades_pertenece(especialidades, clave))){
            especialidades->tabla[pos].clave = clave_copia;
            especialidades->tabla[pos].cola_urgente = cola_urgente;
            especialidades->tabla[pos].pacientes_espera = en_espera;
            especialidades->cant++;
            return true;
        }else if (especialidades->tabla[pos].estado == OCUPADO && strcmp(especialidades->tabla[pos].clave,clave_copia) == 0){
            if (especialidades->destruir_cola){
                cola_destruir(especialidades->tabla[pos].cola_urgente, especialidades->destruir_cola);
            }
            if (especialidades->destruir_heap){
                heap_destruir(especialidades->tabla[pos].pacientes_espera, especialidades->destruir_heap);
            }
            especialidades->tabla[pos].cola_urgente = cola_urgente;
            especialidades->tabla[pos].pacientes_espera = en_espera;
            free(clave_copia);
            return true;
        }
        pos = (pos + 5 * i) % especialidades->tam;       
    }
    return false;
}

void *especialidades_obtener_dato(const especialidades_t *especialidades, const char *clave, char* tipo_dato){
    unsigned long pos = f_hash(clave) % especialidades->tam;
    if(!especialidades_pertenece(especialidades, clave)) return NULL;
    for (size_t i = 0; i < especialidades->tam; i++){
        pos = (pos + 5 * i) % especialidades->tam;
        if (especialidades->tabla[pos].estado == OCUPADO && strcmp(especialidades->tabla[pos].clave,clave) == 0){
            if (strcmp(tipo_dato, URGENCIA) == 0) {
                return especialidades->tabla[pos].cola_urgente;
            } 
            else if (strcmp(tipo_dato, REGULAR) == 0) {
                return especialidades->tabla[pos].pacientes_espera;
            }
            else {
                return NULL;
            }
            
        }
        else if(especialidades->tabla[pos].estado  == VACIO){
            return NULL;
        }
    }    
    return NULL;
}

bool especialidades_pertenece(const especialidades_t *especialidades, const char *clave){
    unsigned long pos = f_hash(clave) % especialidades->tam;
    for (size_t i = 0; i < especialidades->tam; i++){
        pos = (pos + 5 * i) % especialidades->tam;
        if (especialidades->tabla[pos].estado == OCUPADO && strcmp(especialidades->tabla[pos].clave,clave) == 0){
            return true;
        }
        else if(especialidades->tabla[pos].estado  == VACIO){
            return false;
        }
    } 
    return false;
}

size_t especialidades_cantidad(const especialidades_t *especialidades){
    return especialidades->cant;
}

void especialidades_destruir(especialidades_t *especialidades){
    size_t pos = 0;
    while(pos < especialidades->tam){
        if (especialidades->tabla[pos].estado == OCUPADO){
            if(especialidades->destruir_cola){ 
                cola_destruir(especialidades->tabla[pos].cola_urgente, especialidades->destruir_cola);
            }
            if(especialidades->destruir_heap){
                heap_destruir(especialidades->tabla[pos].pacientes_espera, especialidades->destruir_heap);
            } 
            free(especialidades->tabla[pos].clave);
        }
        pos++;
    }
    free(especialidades->tabla);
    free(especialidades);
}

typedef struct especialidades_iter{
    size_t pos;
    const especialidades_t* especialidades;
} especialidades_iter_t;

especialidades_iter_t *especialidades_iter_crear(const especialidades_t *especialidades){
    especialidades_iter_t* iter = malloc(sizeof(especialidades_iter_t));
    if (!(iter)) return NULL;
    iter->especialidades = especialidades;
    size_t pos = 0;
    while (pos < especialidades->tam && especialidades->tabla[pos].estado != OCUPADO){
        pos++;
    }
    iter->pos = pos;
    return iter;
}

bool especialidades_iter_al_final(const especialidades_iter_t *iter){
    if (iter->pos != iter->especialidades->tam){
        return false;
    }
    return true; 
}

bool especialidades_iter_avanzar(especialidades_iter_t *iter){
    if (especialidades_iter_al_final(iter)) return false;
    iter->pos++; 
    while (!(especialidades_iter_al_final(iter))){
        if (iter->especialidades->tabla[iter->pos].estado == OCUPADO) return true;
        iter->pos++;
    }
    return true;
}


const char *especialidades_iter_ver_actual(const especialidades_iter_t *iter){
    if (iter->pos != iter->especialidades->tam){
        return iter->especialidades->tabla[iter->pos].clave;
    }
    return NULL;
}

void especialidades_iter_destruir(especialidades_iter_t *iter){
    free(iter);
}