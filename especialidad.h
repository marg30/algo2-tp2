#include <stdbool.h>
#include <stddef.h>
#include "paciente.h"
#include "heap.h"

struct especialidad;

typedef struct especialidad especialidad_t;

typedef void (*especialidades_destruir_dato_t)(void *);

//Crea un puntero a especialidad. Recibe como parametros una funcion de destruccion para pacientes urgentes y otra para no urgentes
//Post: la estructura especialidad fue inicializada
especialidad_t* especialidad_crear(especialidades_destruir_dato_t destruir_cola, especialidades_destruir_dato_t destruir_heap, cmp_func_t cmp);

//Destruye la estructura especialidad liberando la memoria pedida
//Pre: La estructura especialidad fue inicializada
//Post: La estructura especialidad fue destruida
void especialidad_destruir(especialidad_t* especialidad);

//Guarda el turno de un paciente pasado por parametro. Tambien recibe como parametro si se trata de un caso urgente o no. Devuelve false si falla
//Pre: La estructura especialidad fue inicializada
//Post: El turno solicitado fue guardado
bool guardar_turno(especialidad_t* especialidad, paciente_t* paciente, bool urgente);