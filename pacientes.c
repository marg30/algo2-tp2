#include "testing.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pacientes.h"
#include "f_hash.h"
#define CORRECTOR 

#define FACTOR_DE_CARGA 0.7
#define TAM_INICIAL 23

// ******************************************************************
//                        PROGRAMA PRINCIPAL


typedef void (*pacientes_destruir_dato_t)(void *);

enum campo_estado{
    VACIO,
    OCUPADO,
    BORRADO,
};

typedef struct campo_pacientes{
    char* clave;
    int* fecha_inscripcion;
    enum campo_estado estado;
}campo_pacientes_t;

typedef struct pacientes{
    campo_pacientes_t* tabla;
    size_t tam;
    size_t cant;
    pacientes_destruir_dato_t destruir_dato; 
}pacientes_t;

campo_pacientes_t *crear_tabla_pacientes(size_t capacidad){
    campo_pacientes_t *tabla = malloc(sizeof(campo_pacientes_t)*capacidad);
	if(!(tabla)) return NULL;
    return tabla;
}

/*int cmp(campo_pacientes_t* paciente_1, campo_pacientes_t* paciente_2){
    if (paciente_1->fecha_inscripcion > paciente_2->fecha_inscripcion){
        return -1;
    }else if(paciente_1->fecha_inscripcion < paciente_2->fecha_inscripcion){
        return 1;
    }
    return 0;
}*/

pacientes_t *pacientes_crear(pacientes_destruir_dato_t destruir_dato){
    pacientes_t* pacientes = malloc(sizeof(pacientes_t));
    if(!(pacientes)) return NULL;
    pacientes->tabla = crear_tabla_pacientes(TAM_INICIAL);
    if(!(pacientes->tabla)){
        free(pacientes);
        return NULL;
    }
    size_t pos = 0;
    pacientes->tam = TAM_INICIAL;
    while(pos < pacientes->tam){
        pacientes->tabla[pos].estado = VACIO;
        pos++;
    }
    pacientes->cant =  0;
    pacientes->destruir_dato = destruir_dato;
    return pacientes;
}

bool pacientes_redimensionar(pacientes_t *pacientes, size_t capacidad){

    campo_pacientes_t *tabla_ant = pacientes->tabla;
    size_t tam_ant = pacientes->tam; 
    pacientes->tam = capacidad;
    pacientes->tabla = crear_tabla_pacientes(capacidad);
    
    if (pacientes->tabla == NULL) return false;

    size_t pos = 0;
	
    pacientes->cant = 0;
    while(pos < pacientes->tam){
        pacientes->tabla[pos].clave = NULL;
		pacientes->tabla[pos].fecha_inscripcion = NULL;
        pacientes->tabla[pos].estado = VACIO;
        pos++;
    }

    bool redimension_ok = true;

    for (size_t i = 0; i < tam_ant; i++) {
        if(tabla_ant[i].estado == OCUPADO){
            void *inscripcion = tabla_ant[i].fecha_inscripcion;
            void *clave = tabla_ant[i].clave;
            redimension_ok = pacientes_guardar(pacientes, clave, inscripcion);
            free(clave);
        }
    }

    free(tabla_ant);
    return redimension_ok;
}

bool pacientes_guardar(pacientes_t *pacientes, const char *clave, int *fecha){
    if ((pacientes->cant)*100 / pacientes->tam >= FACTOR_DE_CARGA * 100 ){
        if(!pacientes_redimensionar(pacientes, pacientes->tam*2)) return false;
    }
    char* clave_copia = strdup(clave);
    if(!(clave_copia)) return false;
    unsigned long pos = f_hash(clave_copia) % pacientes->tam;
    for (size_t i = 0; i < pacientes->tam; i++){
        if (pacientes->tabla[pos].estado != OCUPADO && !(pacientes_pertenece(pacientes, clave))){
            pacientes->tabla[pos].clave = clave_copia;
            pacientes->tabla[pos].fecha_inscripcion = fecha;
            pacientes->tabla[pos].estado = OCUPADO;
            pacientes->cant++;
            return true;
        }else if (pacientes->tabla[pos].estado == OCUPADO && strcmp(pacientes->tabla[pos].clave,clave_copia) == 0){
            pacientes_destruir_dato_t destruir_dato = pacientes->destruir_dato;
            if (destruir_dato){
                destruir_dato((pacientes->tabla[pos].fecha_inscripcion));
            }
            pacientes->tabla[pos].fecha_inscripcion = fecha;
            free(clave_copia);
            return true;
        }
        pos = (pos + 5 * i) % pacientes->tam;       
    }
    return false;
}

void *pacientes_borrar(pacientes_t *pacientes, const char *clave){
    unsigned long pos = f_hash(clave) % pacientes->tam;
    if(pacientes->tabla[pos].estado == VACIO) return NULL;
    for (size_t i = 0; i < pacientes->tam; i++){
        pos = (pos + 5 * i) % pacientes->tam;
        if (pacientes->tabla[pos].estado == OCUPADO && strcmp(pacientes->tabla[pos].clave,clave) == 0){
            free(pacientes->tabla[pos].clave);
            void* dato_borrado = pacientes->tabla[pos].fecha_inscripcion;
            pacientes->tabla[pos].fecha_inscripcion = NULL;
            pacientes->tabla[pos].estado = BORRADO;
            pacientes->cant--;
            return dato_borrado;
        } else if (pacientes->tabla[pos].estado == VACIO){
            return NULL;
        }
    }        
    return NULL;
}

void *pacientes_obtener_fecha(const pacientes_t *pacientes, const char *clave){
    unsigned long pos = f_hash(clave) % pacientes->tam;
    if(!pacientes_pertenece(pacientes, clave)) return NULL;
    for (size_t i = 0; i < pacientes->tam; i++){
        pos = (pos + 5 * i) % pacientes->tam;
        if (pacientes->tabla[pos].estado == OCUPADO && strcmp(pacientes->tabla[pos].clave,clave) == 0){
            return pacientes->tabla[pos].fecha_inscripcion;
        }
        else if(pacientes->tabla[pos].estado  == VACIO){
            return NULL;
        }
    }    
    return NULL;
}

bool pacientes_pertenece(const pacientes_t *pacientes, const char *clave){
    unsigned long pos = f_hash(clave) % pacientes->tam;
    for (size_t i = 0; i < pacientes->tam; i++){
        pos = (pos + 5 * i) % pacientes->tam;
        if (pacientes->tabla[pos].estado == OCUPADO && strcmp(pacientes->tabla[pos].clave,clave) == 0){
            return true;
        }
        else if(pacientes->tabla[pos].estado  == VACIO){
            return false;
        }
    } 
    return false;
}

size_t pacientes_cantidad(const pacientes_t *pacientes){
    return pacientes->cant;
}

void pacientes_destruir(pacientes_t *pacientes){
    pacientes_destruir_dato_t destruir_dato = pacientes->destruir_dato;
    size_t pos = 0;
    while(pos < pacientes->tam){
        if (pacientes->tabla[pos].estado == OCUPADO){
            if(destruir_dato != NULL){ 
                destruir_dato(pacientes->tabla[pos].fecha_inscripcion); 
            }
            free(pacientes->tabla[pos].clave);
        }
        pos++;
    }
    free(pacientes->tabla);
    free(pacientes);
}

typedef struct pacientes_iter{
    size_t pos;
    const pacientes_t* pacientes;
} pacientes_iter_t;

pacientes_iter_t *pacientes_iter_crear(const pacientes_t *pacientes){
    pacientes_iter_t* iter = malloc(sizeof(pacientes_iter_t));
    if (!(iter)) return NULL;
    iter->pacientes = pacientes;
    size_t pos = 0;
    while (pos < pacientes->tam && pacientes->tabla[pos].estado != OCUPADO){
        pos++;
    }
    iter->pos = pos;
    return iter;
}

bool pacientes_iter_al_final(const pacientes_iter_t *iter){
    if (iter->pos != iter->pacientes->tam){
        return false;
    }
    return true; 
}

bool pacientes_iter_avanzar(pacientes_iter_t *iter){
    if (pacientes_iter_al_final(iter)) return false;
    iter->pos++; 
    while (!(pacientes_iter_al_final(iter))){
        if (iter->pacientes->tabla[iter->pos].estado == OCUPADO) return true;
        iter->pos++;
    }
    return true;
}


const char *pacientes_iter_ver_actual(const pacientes_iter_t *iter){
    if (iter->pos != iter->pacientes->tam){
        return iter->pacientes->tabla[iter->pos].clave;
    }
    return NULL;
}

void pacientes_iter_destruir(pacientes_iter_t *iter){
    free(iter);
}