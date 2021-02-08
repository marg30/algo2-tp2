#ifndef PACIENTES_H
#define PACIENTES_H

#include <stdbool.h>
#include <stddef.h>

struct pacientes;
struct pacientes_iter;

typedef struct pacientes pacientes_t;
typedef struct pacientes_iter pacientes_iter_t;

// tipo de función para destruir dato
typedef void (*pacientes_destruir_dato_t)(void *);

/* Crea diccionario de pacientes
 */
pacientes_t *pacientes_crear(pacientes_destruir_dato_t destruir_dato);




/* Guarda un elemento en el diccionario de pacientes, si la 
 * clave ya se encuentra en la estructura, la reemplaza. 
 * De no poder guardarlo devuelve false.
 * Pre: La estructura diccionario de pacientes fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool pacientes_guardar(pacientes_t *pacientes, const char *clave, int *fecha);


bool pacientes_redimensionar(pacientes_t *pacientes, size_t capacidad);

/* Borra un elemento del pacientes y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura diccionario de pacientes fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *pacientes_borrar(pacientes_t *pacientes, const char *clave);

/* Obtiene la fecha de inscripcion de un paciente, 
 * si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura diccionario de pacientes fue inicializada
 */
void *pacientes_obtener_fecha(const pacientes_t *pacientes, const char *clave);

/* Determina si el paciente pertenece o no al diccionario de pacientes.
 * Pre: La estructura diccionario de pacientes fue inicializada
 */
bool pacientes_pertenece(const pacientes_t *pacientes, const char *clave);

/* Devuelve la cantidad de pacientes en el diccionario.
 * Pre: La estructura diccionario de pacientes fue inicializada
 */
size_t pacientes_cantidad(const pacientes_t *pacientes);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura diccionario de pacientes fue inicializada
 * Post: La estructura diccionario de pacientes fue destruida
 */
void pacientes_destruir(pacientes_t *pacientes);

/* Iterador del pacientes */

// Crea iterador
pacientes_iter_t *pacientes_iter_crear(const pacientes_t *pacientes);

// Avanza iterador
bool pacientes_iter_avanzar(pacientes_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *pacientes_iter_ver_actual(const pacientes_iter_t *iter);

// Comprueba si terminó la iteración
bool pacientes_iter_al_final(const pacientes_iter_t *iter);

// Destruye iterador
void pacientes_iter_destruir(pacientes_iter_t *iter);

#endif  // PACIENTES_H