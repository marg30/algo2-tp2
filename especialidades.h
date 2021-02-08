#ifndef ESPECIALIDADES_H
#define ESPECIALIDADES_H

#include <stdbool.h>
#include <stddef.h>
#include "cola.h"
#include "heap.h"

struct especialidades;
struct especialidades_iter;

typedef struct especialidades especialidades_t;
typedef struct especialidades_iter especialidades_iter_t;

// tipo de función para destruir dato
typedef void (*especialidades_destruir_dato_t)(void *);

/* Crea diccionario de especialidades
 */
especialidades_t *especialidades_crear(especialidades_destruir_dato_t destruir_cola, especialidades_destruir_dato_t destruir_heap);




/* Guarda un elemento en el diccionario especialidades, si la 
 * clave ya se encuentra en la estructura, la reemplaza. 
 * De no poder guardarlo devuelve false.
 * Pre: La estructura diccionario de especialidades fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool especialidades_guardar(especialidades_t *especialidades, const char *clave, cola_t *cola_urgente, heap_t *en_espera);


bool especialidades_redimensionar(especialidades_t *especialidades, size_t capacidad);

/* Obtiene el valor de un elemento del diccionario de especialidades, 
 * si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura diccionario de especialidades fue inicializada
 */
void *especialidades_obtener_dato(const especialidades_t *especialidades, const char *clave, char* tipo_dato);

/* Determina si el paciente pertenece o no al diccionario de especialidades.
 * Pre: La estructura diccionario de especialidades fue inicializada
 */
bool especialidades_pertenece(const especialidades_t *especialidades, const char *clave);

/* Devuelve la cantidad de elementos del especialidades.
 * Pre: La estructura diccionario de especialidades fue inicializada
 */
size_t especialidades_cantidad(const especialidades_t *especialidades);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura diccionario de especialidades fue inicializada
 * Post: La estructura diccionario de especialidades fue destruida
 */
void especialidades_destruir(especialidades_t *especialidades);

/* Iterador del especialidades */

// Crea iterador
especialidades_iter_t *especialidades_iter_crear(const especialidades_t *especialidades);

// Avanza iterador
bool especialidades_iter_avanzar(especialidades_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *especialidades_iter_ver_actual(const especialidades_iter_t *iter);

// Comprueba si terminó la iteración
bool especialidades_iter_al_final(const especialidades_iter_t *iter);

// Destruye iterador
void especialidades_iter_destruir(especialidades_iter_t *iter);

#endif  // ESPECIALIDADES_H