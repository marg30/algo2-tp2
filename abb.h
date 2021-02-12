#ifndef ABB_H
#define ABB_H
#endif

#include <stdbool.h>
#include <stddef.h>
typedef struct abb abb_t;
typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

// Crea un abb
// Post: El arbol fue creado
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Guarda un nodo con una clave y un dato asociados. Si la clave ya estaba en el arbol, destruye el anterior dato y guarda el nuevo. Devuelve false si falla
// Pre: El arbol fue creado
// Post: Si la clave es nueva se agrego un nuevo nodo con esa clave y un dato, si no, se cambio el dato asociado a esa clave
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Elimina el nodo asociado a la clave ingresada y devuelve su dato. Devuelve NULL si la clave no esta en el ABB
// Pre: El arbol fue creado
// Pos: Si la clave estaba en el arbol, se borro su nodo y se devolvio su dato
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene el valor asociado a la clave ingresada. Devuelve NULL si la clave no pertenece al arbol
// Pre: El arbol fue creado
// Pos: Si la clave estaba en el arbol, se devolvio su dato.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Devuelve true si la clave ingresada se encuentra en el arbol, false en caso contrario
// Pre: El arbol fue creado
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de nodos que tiene el arbol
// Pre: El arbol fue creado
// Pos: Se devolvio la cantidad de nodos  del ABB
size_t abb_cantidad(abb_t *arbol);

// Destruye el ABB. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los datos de los nodos llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos del ABB.
void abb_destruir(abb_t *arbol);

// Itera el arbol in-order (Primero hijo izquierdo, luego uno mismo, luego hijo derecho). Aplica la funcion visitar a los elementos sobre los cual itere
// En extra se guardan valores que se puedan necesitar al recorrer el arbol
// Pre: El arbol fue creada. Visitar es una funcion que capaz de actuar sobre los datos del arbol.
// Post: El arbol se recorre y se aplica visitar hasta que esta devuelva false. Si no lo hace, se recorre entero
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *, void *, void *), void *extra, void *limite_1, void *limite_2);


typedef struct abb_iter abb_iter_t;

// Crea un iterador para el ABB pasado por parametro.
// Pre: El arbol fue creado
// Pos: El iterador fue creado
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// El iterador avanza al nodo siguiente del recorrido in-order. Devuelve false si no se puede avanzar mas
// Pre: El iterador fue creado
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve el dato ubicado en la posicion del iterador. Devuelve NULL si no hay dato
// Pre: El iterador fue creado
// Pos: Se devolvio el dato de la posicion del iterador, si el iterador no esta al final
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Devuelve true si el iterador esta al final del arbol, es decir la posicion siguiente al ultimo del recorrdo in-order
// Pre: El iterador fue creado
// Pos: Se devolvio un booleano que indica si el iterador esta al final del arbol
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador
// Pre: El iterador fue creado
// Pos: Se eliminaron los datos del iterador
void abb_iter_in_destruir(abb_iter_t* iter);
