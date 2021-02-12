#define _POSIX_C_SOURCE 200809L //getline
#include "strutil.h"
#include "hash.h"
#include "abb.h"
#include "paciente.h"
#include "doctor.h"
#include "especialidad.h"
#include "mensajes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool csv_doctores_crear_estructura(const char* ruta_csv, abb_t* doctores, hash_t* especialidades);

int comparar_pacientes(const void* paciente_1, const void* paciente_2);

bool csv_pacientes_crear_estructura(const char* ruta_csv, hash_t* pacientes);


void procesar_archivos(const char* ruta_csv_doctores, const char* ruta_csv_pacientes, abb_t* doctores, hash_t* especialidades, hash_t* pacientes);

void eliminar_fin_linea(char* linea);