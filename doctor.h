#ifndef ABB_H
#define ABB_H
#endif

#include <stdbool.h>
#include <stddef.h>
typedef struct doctores doctores_t;
typedef int (*doctores_comparar_nombre_t) (const char *, const char *);
typedef void (*doctores_destruir_dato_t) (void *);

// Crea un abb
// Post: El arbol fue creado
doctores_t* doctores_crear(doctores_comparar_nombre_t cmp, doctores_destruir_dato_t destruir_dato);

// Guarda un nodo con una clave y un dato asociados. Si la clave ya estaba en el arbol, destruye el anterior dato y guarda el nuevo. Devuelve false si falla
// Pre: El arbol fue creado
// Post: Si la clave es nueva se agrego un nuevo nodo con esa clave y un dato, si no, se cambio el dato asociado a esa clave
bool doctores_guardar(doctores_t *doctores, const char *nombre, void *dato);

// Elimina el nodo asociado a la clave ingresada y devuelve su dato. Devuelve NULL si la clave no esta en el ABB
// Pre: El arbol fue creado
// Pos: Si la clave estaba en el arbol, se borro su nodo y se devolvio su dato
void *doctores_borrar(doctores_t *doctores, const char *nombre);

// Obtiene la especialidad asociada a un doctor. Devuelve NULL si el doctor no pertenece al arbol
// Pre: El arbol de doctores fue creado
// Pos: Si la clave estaba en el arbol, se devolvio su dato.
especialidades_t* doctores_obtener_especialidad(const doctores_t* doctores, const char* nombre);

// Obtiene la cantidad de pacientes atendidos por un doctor. Devuelve NULL si el doctor no pertenece al arbol
// Pre: El arbol de doctores fue creado
// Pos: Si la clave estaba en el arbol, se devolvio su dato.
int doctores_obtener_pacientes_atendidos(const doctores_t* doctores, const char* nombre);


// Devuelve true si la clave ingresada se encuentra en el arbol, false en caso contrario
// Pre: El arbol de doctores fue creado
bool doctores_pertenece(const doctores_t *doctores, const char *clave);

// Devuelve la cantidad de doctores que tiene el arbol
// Pre: El arbol de doctores fue creado
// Pos: Se devolvio la cantidad de doctores del ABB
size_t doctores_cantidad(doctores_t *doctores);

// Destruye el ABB de doctores. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los datos de los nodos llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos del ABB.
void doctores_destruir(doctores_t* doctores);

// Itera el arbol in-order (Primero hijo izquierdo, luego uno mismo, luego hijo derecho). Aplica la funcion visitar a los elementos sobre los cual itere
// En extra se guardan valores que se puedan necesitar al recorrer el arbol
// Pre: El arbol fue creada. Visitar es una funcion que capaz de actuar sobre los datos del arbol.
// Post: El arbol se recorre y se aplica visitar hasta que esta devuelva false. Si no lo hace, se recorre entero
void doctores_in_order(doctores_t *doctores, bool visitar(const char *, void *, void *), void *extra);

typedef struct doctores_iter doctores_iter_t;

// Crea un iterador para el ABB de doctores pasado por parametro.
// Pre: El arbol de doctores fue creado
// Pos: El iterador fue creado
doctores_iter_t *doctores_iter_in_crear(const doctores_t *doctores);

// El iterador avanza al nodo siguiente del recorrido in-order. Devuelve false si no se puede avanzar mas
// Pre: El iterador fue creado
bool doctores_iter_in_avanzar(doctores_iter_t *iter);

// Devuelve el nombre ubicado en la posicion del iterador. Devuelve NULL si no hay dato
// Pre: El iterador fue creado
// Pos: Se devolvio el dato de la posicion del iterador, si el iterador no esta al final
const char *doctores_iter_in_ver_actual(const doctores_iter_t *iter);

// Devuelve true si el iterador esta al final del arbol, es decir la posicion siguiente al ultimo del recorrdo in-order
// Pre: El iterador fue creado
// Pos: Se devolvio un booleano que indica si el iterador esta al final del arbol
bool doctores_iter_in_al_final(const doctores_iter_t *iter);

// Destruye el iterador
// Pre: El iterador fue creado
// Pos: Se eliminaron los datos del iterador
void doctores_iter_in_destruir(doctores_iter_t* iter);