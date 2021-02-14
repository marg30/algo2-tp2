#define _POSIX_C_SOURCE 200809L // strdup
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "especialidad.h"
#include "paciente.h"
#include "cola.h"
#include "heap.h"

typedef struct especialidad{
    cola_t* cola_urgentes;
    heap_t* heap_no_urgentes;
    especialidades_destruir_dato_t destruir_cola;
    especialidades_destruir_dato_t destruir_heap;
    size_t pacientes_en_espera;
}especialidad_t;

typedef void (*especialidades_destruir_dato_t)(void *);

/*int cmp(paciente_t* paciente_1, paciente_t* paciente_2){
    if (paciente1->anio_inscripcion > paciente2->anio_inscripcion){
        return -1;
    }else if(paciente1->anio_inscripcion < paciente2->anio_inscripcion){
        return 1;
    }
    return 0;
}*/

especialidad_t* especialidad_crear(especialidades_destruir_dato_t destruir_cola, especialidades_destruir_dato_t destruir_heap, cmp_func_t cmp){
    especialidad_t* especialidad = malloc(sizeof(especialidad_t));
    if(!(especialidad)) return NULL;
    especialidad->cola_urgentes = cola_crear();
    if(!(especialidad->cola_urgentes)) {
        free(especialidad);
        return NULL;
    }
    especialidad->heap_no_urgentes = heap_crear(cmp);
    if(!(especialidad->heap_no_urgentes)) {
        free(especialidad);
        free(especialidad->cola_urgentes);
        return NULL;
    }
    especialidad->destruir_cola = destruir_cola;
    especialidad->destruir_heap = destruir_heap;
    especialidad->pacientes_en_espera = 0;
    
    return especialidad;
}

size_t especialidad_cant_en_espera(especialidad_t* especialidad){
    return especialidad->pacientes_en_espera;
}

void especialidad_destruir(especialidad_t* especialidad){
    cola_destruir(especialidad->cola_urgentes, especialidad->destruir_cola);
    heap_destruir(especialidad->heap_no_urgentes, especialidad->destruir_heap);
    free(especialidad);
}

bool guardar_turno(especialidad_t* especialidad, paciente_t* paciente, bool urgente){
    if(urgente){
        if(!(cola_encolar(especialidad->cola_urgentes, paciente))) return false;
    }else{
        if(!(heap_encolar(especialidad->heap_no_urgentes, paciente))) return false;
    }
    especialidad->pacientes_en_espera++;
    return true;
}

void* atender_paciente(especialidad_t* especialidad){
    if(!(cola_esta_vacia(especialidad->cola_urgentes))){
        especialidad->pacientes_en_espera--;
        return cola_desencolar(especialidad->cola_urgentes);
    }else if(!(heap_esta_vacio(especialidad->heap_no_urgentes))){
        especialidad->pacientes_en_espera--;
        return heap_desencolar(especialidad->heap_no_urgentes);
    }
    return NULL;
}