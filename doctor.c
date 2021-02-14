#define _POSIX_C_SOURCE 200809L // strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "doctor.h"

typedef struct doctor{
    char* especialidad;
    size_t pacientes_atendidos;
}doctor_t;

doctor_t* doctor_crear(char* especialidad){
    doctor_t* doctor = malloc(sizeof(doctor_t));
    if(!doctor){
        free(doctor);
        return NULL;
    }
    doctor->especialidad = strdup(especialidad);
    if(!doctor->especialidad){
        free(doctor->especialidad);
        return NULL;
    }
    doctor->pacientes_atendidos = 0;

    return doctor;
}

void doctor_sumar_atendido(doctor_t* doctor){
    doctor->pacientes_atendidos++;
}

size_t doctor_ver_atendidos(doctor_t* doctor){
    return doctor->pacientes_atendidos;
}

void doctor_destruir(doctor_t* doctor){
    free(doctor->especialidad);
    free(doctor);
}

char* doctor_obtener_especialidad(doctor_t *doctor){
    return doctor->especialidad;
}