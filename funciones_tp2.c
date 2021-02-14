#include <stdio.h>
#include "doctor.h"
#include "especialidad.h"
#include "paciente.h"
#include "testing.h"
#include "mensajes.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

bool imprimir_doctor(const char* clave, void* dato, void* extra, void* inicio, void* fin) {
	size_t* contador = extra;
	doctor_t* doc = (doctor_t*) dato;
	char* especialidad = doctor_obtener_especialidad(doc);
	size_t pacientes_atendidos = doctor_ver_atendidos(doc);
	if (strlen(inicio) == 0 || strcmp(inicio, clave) <= 0) {
		if (strlen(fin) != 0 && strcmp(fin, clave) < 0) {
			return false;
		}
    	printf(INFORME_DOCTOR, ++(*contador), clave, especialidad, pacientes_atendidos);
	}

    return true;
}

bool calc_distancia(const char* clave, void* dato, void* extra, void* inicio, void* fin) {
	int* contador = extra;
	if (strlen(inicio) == 0 || strcmp(inicio, clave) <= 0) {
		if (strlen(fin) != 0 && strcmp(fin, clave) < 0) {
			return false;
    	}
    	(*contador)++;
		*(int *) extra = *contador;
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