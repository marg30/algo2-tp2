#define _POSIX_C_SOURCE 200809L // strdup
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "paciente.h"

typedef struct paciente{
    char* nombre;
    int anio_inscripcion;
}paciente_t;

int cmp_pacientes(const paciente_t* paciente_1, const paciente_t* paciente_2){
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
    paciente->nombre = strdup(nombre);
    paciente->anio_inscripcion = anio_inscripcion;
    
    return paciente; 
}

char *paciente_obtener_nombre(paciente_t* paciente){
    return paciente->nombre;
}

int paciente_ver_anio_insc(paciente_t* paciente){
    return paciente->anio_inscripcion;
}

void paciente_destruir(paciente_t* paciente){
    free(paciente);
}