#include <stdbool.h>
#include <stddef.h>

struct doctor;

typedef struct doctor doctor_t;

typedef void (*hash_destruir_dato_t)(void *);

//Crea un puntero a doctor. Recibe como unico parametro su especialidad
//Post: La estructura doctor fue inicializada
doctor_t* doctor_crear(char* especialidad);

//Suma 1 a la cantidad de pacientes atendidos por el doctor
//Pre: La estructura doctor fue inicializada
void doctor_sumar_atendido(doctor_t* doctor);

//Devuelve la cantidad de atendidos del doctor
//Pre: La estructura doctor fue inicializada
int doctor_ver_atendidos(doctor_t* doctor);

//Destruye la estructura doctor liberando la memoria pedida
//Pre: La estructura doctor fue inicializada
//Post: La estructura doctor fue destruida
void doctor_destruir(doctor_t* doctor);