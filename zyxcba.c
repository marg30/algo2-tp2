#define _POSIX_C_SOURCE 200809L // strdup
#include <stdio.h>
#include <string.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "doctor.h"
#include "abb.h"
#include "especialidad.h"
#include <stdlib.h>

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

bool imprimir_doctor(const char* clave, void* dato, void* extra, void* inicio, void* fin) {
	int* contador = extra;
	doctor_t* doc = (doctor_t*) dato;
	char* especialidad = doctor_obtener_especialidad(doc);
	int pacientes_atendidos = doctor_ver_atendidos(doc);
	if (strlen(inicio) == 0 || strcmp(inicio, clave) <= 0) {
    	printf(INFORME_DOCTOR, ++(*contador), clave, especialidad, pacientes_atendidos);
	}

    if (strlen(fin) != 0 && strcmp(fin, clave) == 0) {
        return false;
    }
    return true;
}

int calc_distancia(const char* clave, void* dato, void* extra, void* inicio, void* fin) {
	int* contador = extra;
	if (strlen(inicio) == 0 || strcmp(inicio, clave) <= 0) {
    	(*contador)++;
		*(int *) extra = *contador;
	}

    if (strlen(extra) != 0 && strcmp(fin, clave) == 0) {
		*contador++;
        return false;
    }
	return true;
}

size_t largo_array_de_arrays(const char** array) {
	size_t i = 0;
	while (array[i] != '\0'){
		i++;
	}
	return i;
}
bool es_formato_correcto(const char** parametros, abb_t* doctores){
	if (largo_array_de_arrays(parametros) != 2) {
		printf(ENOENT_PARAMS, COMANDO_INFORME);
		return false;
	}
	if (strlen(parametros[0]) != 0 && !abb_pertenece(doctores, parametros[0])) {
		printf(ENOENT_DOCTOR, parametros[0]);
		return false;
	}
	if (strlen(parametros[1]) != 0 && !abb_pertenece(doctores, parametros[1])) {
		printf(ENOENT_DOCTOR, parametros[1]);
		return false;
	}
	return true;
}



void obtener_informe_doctores(const char** parametros, abb_t* doctores){
	if (es_formato_correcto(parametros, doctores)) {
		size_t n_doctores = 0;
		abb_in_order(doctores, calc_distancia, &n_doctores, parametros[0], parametros[1]);
		printf(DOCTORES_SISTEMA, n_doctores);
		size_t contador = 0;
    	void* extra = &contador;
		abb_in_order(doctores, imprimir_doctor, &contador, parametros[0], parametros[1]);
	}
}

void pedir_turno(){

}

void atender_paciente(){
	
}


abb_t* de_archivo_a_estructura_doctores(char *nombre_archivo){
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == -1) {
        return NULL;
    }
	
	char *linea = NULL;
    size_t capacidad = 0;
    abb_t *doctores = abb_crear(strcmp, free);

    while (getline(&linea, &capacidad, archivo) != EOF){
        eliminar_fin_linea(linea);
        char** campos = split(linea, ',');
		doctor_t *doctor = doctor_crear(campos[1]);
        abb_guardar(doctores, campos[0], doctor);
        free_strv(campos);
    }
    free(linea);
	return doctores;
}

void procesar_comando(const char* comando, const char** parametros, abb_t *doctores) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		pedir_turno(parametros, doctores);

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		atender_paciente(parametros, doctores);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		obtener_informe_doctores(parametros, doctores);
	} else {
		printf(ENOENT_CMD, comando);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada() {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		abb_t *doctores = de_archivo_a_estructura_doctores("doctores.csv");
		procesar_comando(campos[0], parametros, doctores);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}



int main(int argc, char** argv) {
	procesar_entrada();
	return 0;
}