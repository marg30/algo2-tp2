#define _POSIX_C_SOURCE 200809L // strdup
#include <stdio.h>
#include <string.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "doctor.h"
#include "abb.h"
#include "diccionarios.h"
#include "hash.h"
#include "especialidad.h"
#include <stdlib.h>

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define N_PARAM_TURNO 3
#define N_PARAM_ATENDER 1
#define N_PARAM_INFORME 2
#define URGENTE "URGENTE"
#define REGULAR "REGULAR"

bool es_formato_comando(const char** parametros, int cant_correcta){
	if (largo_array_de_arrays(parametros) != cant_correcta) {
		printf(ENOENT_PARAMS, COMANDO_INFORME);
		return false;
	}
	return true;
}

bool es_formato_correcto_pedir_turno(const char** parametros, hash_t* especialidades, hash_t* pacientes){
	bool es_formato_correcto = true;
	if(!hash_pertenece(pacientes, parametros[0])){
		printf(ENOENT_PACIENTE, parametros[0]);
		es_formato_correcto = false;
	}
	if(!hash_pertenece(especialidades, parametros[1])){
		printf(ENOENT_ESPECIALIDAD, parametros[1]);
		es_formato_correcto = false;
	}
	if(strcmp(parametros[2], URGENTE) != 0 && strcmp(parametros[2], REGULAR) != 0){
		printf(ENOENT_URGENCIA, parametros[2]);
		es_formato_correcto = false;
	}
	return es_formato_correcto;
}

bool determinar_urgencia(const char* urgencia){
	return strcmp(urgencia, URGENTE) == 0;
}

bool es_doctor(char* clave, abb_t *doctores){
	if (strlen(clave) != 0 && !abb_pertenece(doctores, clave)) {
		printf(ENOENT_DOCTOR, clave);
		return false;
	}
	return true;
}

bool es_formato_correcto_atender(const char** parametros, abb_t* doctores) {
	bool es_formato_correcto = true;
	es_formato_correcto = es_doctor(parametros[0], doctores);
	return es_formato_correcto;
}

bool es_formato_correcto_informe_doctores(const char** parametros, abb_t* doctores){
	bool es_formato_correcto = true;
	es_formato_correcto = es_doctor(parametros[0], doctores);
	es_formato_correcto = es_doctor(parametros[1], doctores);
	return es_formato_correcto;
}

void obtener_informe_doctores(const char** parametros, abb_t* doctores){
	if (!es_formato_comando(parametros, N_PARAM_INFORME) || !es_formato_correcto_informe_doctores(parametros, doctores)) {
		return;
	}
	size_t n_doctores = 0;
	abb_in_order(doctores, calc_distancia, &n_doctores, parametros[0], parametros[1]);
	printf(DOCTORES_SISTEMA, n_doctores);
	size_t contador = 0;
	void* extra = &contador;
	abb_in_order(doctores, imprimir_doctor, &contador, parametros[0], parametros[1]);
}

void pedir_turno(const char** parametros, hash_t *especialidades, hash_t *pacientes){
	if (!es_formato_comando(parametros, N_PARAM_TURNO) || !es_formato_correcto_pedir_turno(parametros, especialidades, pacientes)) {
		return;
	} 

	char* nombre_paciente = parametros[0];
	char* nombre_esp = parametros[1];
	char* urgencia = parametros[2];

	especialidad_t *especialidad = hash_obtener(especialidades, nombre_esp);
	paciente_t *paciente = hash_obtener(pacientes, nombre_paciente);

	guardar_turno(especialidad, paciente, determinar_urgencia(urgencia));
	printf(PACIENTE_ENCOLADO, nombre_paciente);
	printf(CANT_PACIENTES_ENCOLADOS, especialidad_cant_en_espera(especialidad), nombre_esp);

}

void cmd_atender_paciente(const char** parametros, abb_t *doctores, hash_t *especialidades){
	if (!es_formato_comando(parametros, N_PARAM_ATENDER) || !es_formato_correcto_atender(parametros, doctores)) {
		return;
	}
	doctor_t *doctor = abb_obtener(doctores, parametros[0]);
	char* nombre_esp = doctor_obtener_especialidad(doctor);
	especialidad_t *esp = hash_obtener(especialidades, nombre_esp);
	paciente_t *paciente = atender_paciente(esp);
	if(!paciente){
		printf(SIN_PACIENTES);
	}
	else {
		//printf(PACIENTE_ATENDIDO, (char*) paciente_obtener_nombre(paciente));
		printf(CANT_PACIENTES_ENCOLADOS, especialidad_cant_en_espera(esp), nombre_esp);
		doctor_sumar_atendido(doctor);
	}

}


void procesar_comando(const char* comando, const char** parametros, abb_t *doctores, hash_t *especialidades, hash_t *pacientes) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		pedir_turno(parametros, especialidades, pacientes);

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		cmd_atender_paciente(parametros, doctores, especialidades);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		obtener_informe_doctores(parametros, doctores);
	} else {
		printf(ENOENT_CMD, comando);
	}
}

void procesar_entrada(abb_t *doctores, hash_t *especialidades, hash_t *pacientes) {
	char* linea = NULL;
	size_t c = 0;
	procesar_archivos("doctores.csv", "pacientes.csv", doctores, especialidades, pacientes);
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros, doctores, especialidades, pacientes);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}



int main(int argc, char** argv) {
	abb_t *doctores = abb_crear(strcmp, free);
	hash_t *especialidades = hash_crear(free);
	hash_t *pacientes = hash_crear(free);
	procesar_entrada(doctores, especialidades, pacientes);
	abb_destruir(doctores);
	hash_destruir(especialidades);
	hash_destruir(pacientes);
	return 0;
}