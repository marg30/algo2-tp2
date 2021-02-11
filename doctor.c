#define _POSIX_C_SOURCE 200809L // strdup
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "doctor.h"

typedef struct doctor{
    char* especialidad;
    int pacientes_atendidos;
}doctor_t;

doctor_t* doctor_crear(char* especialidad){
    doctor_t* doctor = malloc(sizeof(doctor_t));
    doctor->especialidad = strdup(especialidad);
    doctor->pacientes_atendidos = 0;

    return doctor;
}

void doctor_sumar_atendido(doctor_t* doctor){
    doctor->pacientes_atendidos++;
}

int doctor_ver_atendidos(doctor_t* doctor){
    return doctor->pacientes_atendidos;
}

void doctor_destruir(doctor_t* doctor){
    free(doctor);
}

char* doctor_obtener_especialidad(doctor_t *doctor){
    return (char*) doctor->especialidad;
}