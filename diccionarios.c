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

#define SEPARADOR ','

int comparar_pacientes(const void* paciente_1, const void* paciente_2){
	return cmp_pacientes(paciente_1, paciente_2);
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

bool csv_doctores_crear_estructura(const char* ruta_csv, abb_t* doctores, hash_t* especialidades) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		printf(ENOENT_ARCHIVO, ruta_csv);
		fclose(archivo);
		return false;
	}
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
        doctor_t* doctor = doctor_crear(campos[1]);
        abb_guardar(doctores, campos[0], doctor);
        if(!(hash_pertenece(especialidades, campos[1]))){
            especialidad_t* especialidad = especialidad_crear(NULL, NULL, comparar_pacientes);
            hash_guardar(especialidades, campos[1], especialidad);
        }
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return true;
}

bool csv_pacientes_crear_estructura(const char* ruta_csv, hash_t* pacientes) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		printf(ENOENT_ARCHIVO, ruta_csv);
		fclose(archivo);
		return false;
	}
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		int anio = atoi(campos[1]); //atoi pasa de char* a int y devuelve 0 si la conversion no es posible
		if(anio == 0 && strcmp(campos[1], "0") != 0){
			printf(ENOENT_ANIO, campos[1]);
			free_strv(campos);
			free(linea);
			fclose(archivo);
			return false;
		}
        paciente_t* paciente = paciente_crear(campos[0], anio);
        hash_guardar(pacientes, campos[0], paciente);
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return true;
}

void espec_destruir(void* especialidad){
	especialidad_destruir(especialidad);
}

void doc_destruir(void* doctor){
	doctor_destruir(doctor);
}

bool procesar_archivos(const char* ruta_csv_doctores, const char* ruta_csv_pacientes, abb_t* doctores, hash_t* especialidades, hash_t* pacientes){
	bool archivos_validos = true;
	if(!(csv_doctores_crear_estructura(ruta_csv_doctores, doctores, especialidades))){
		archivos_validos = false;
		printf(ENOENT_ARCHIVO, ruta_csv_doctores);
	} 

	if(!(csv_pacientes_crear_estructura(ruta_csv_pacientes, pacientes))) {
		archivos_validos = false;
		printf(ENOENT_ARCHIVO, ruta_csv_pacientes);
	}
	return archivos_validos;
}

//A partir de aca esta lo que hice para probar si funcionaba todo bien y sin perder memoria

/*int main(){
    abb_t* doctores = abb_crear(strcmp, doc_destruir);
	hash_t* especialidades = hash_crear(espec_destruir);
	hash_t* pacientes = hash_crear(free);
	if(csv_doctores_crear_estructura("/home/fede/Documentos/Algoritmos Y Programacion II/TPs/9 - TP2/tp2/doctores.csv", doctores, especialidades)){
		csv_pacientes_crear_estructura("/home/fede/Documentos/Algoritmos Y Programacion II/TPs/9 - TP2/tp2/pacientes.csv", pacientes);
	};
	abb_destruir(doctores);
	hash_destruir(especialidades);
	hash_destruir(pacientes);
	return 0;
}*/

	/*doctor_t* Juana = abb_obtener(doctores, "Juana");
	printf("%s \n", doctor_obtener_especialidad(Juana));
	doctor_sumar_atendido(Juana);
	printf("%i \n", doctor_ver_atendidos(Juana));
	doctor_t* Pepa = abb_obtener(doctores, "Pepa");
	printf("%s \n", doctor_obtener_especialidad(Pepa));
	printf("%i \n", doctor_ver_atendidos(Pepa));
	if(hash_pertenece(especialidades, "Pediatria") && hash_pertenece(especialidades, "Oftalmologia")){
		printf("Pertenecen \n");
	}
	printf("%li \n", hash_cantidad(especialidades));
	paciente_t* Raul = hash_obtener(pacientes, "Raul");
	paciente_t* Mirtha = hash_obtener(pacientes, "Mirtha");
	printf("Raul esta desde %i y Mirtha desde %i \n", paciente_ver_anio_insc(Raul), paciente_ver_anio_insc(Mirtha));*/
