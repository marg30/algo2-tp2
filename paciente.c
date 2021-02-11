#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "paciente.h"

typedef struct paciente{
    char* nombre;
    int anio_inscripcion;
}paciente_t;

int cmp(paciente_t* paciente_1, paciente_t* paciente_2){
    if (paciente_1->anio_inscripcion > paciente_2->anio_inscripcion){
        return -1;
    }else if(paciente_1->anio_inscripcion < paciente_2->anio_inscripcion){
        return 1;
    }
    return 0;
}

paciente_t* paciente_crear(char* nombre, int anio_inscripcion){
    paciente_t* paciente = malloc(sizeof(paciente_t));
    if(!(paciente)) return NULL;
    paciente->nombre = nombre;
    paciente->anio_inscripcion = anio_inscripcion;
    
    return paciente; 
}

void paciente_destruir(paciente_t* paciente){
    free(paciente);
}