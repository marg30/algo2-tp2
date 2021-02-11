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
    if(!(especialidad->cola_urgentes)) return NULL;
    especialidad->heap_no_urgentes = heap_crear(cmp);
    if(!(especialidad->heap_no_urgentes)) return NULL;
    especialidad->destruir_cola = destruir_cola;
    especialidad->destruir_heap = destruir_heap;
    
    return especialidad;
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
    return true;
}