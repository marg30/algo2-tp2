#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/


struct lista;
typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía. Si hubo un fallo al 
// crear la lista, devuelve NULL. 
lista_t *lista_crear(void);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al 
// principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *valor);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al 
//final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *valor);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t *lista);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el tamaño de la lista
// Pre: la lista fue creada
size_t lista_largo(const lista_t *lista);


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 *                             INTERNO
 * *****************************************************************/

// Itera la lista. Si visitar devuelve false, se detiene la iteración de la lista.
// Pre: la lista fue creada. Se llama a la función visitar por cada elemento
// de la lista y tiene ser distinto de NULL. 
// Se usa al parametro extra en la función visitar, como condición de corte, por ejemplo.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 *                             EXTERNO
 * *****************************************************************/

// Crea un iterador externo.
// Pre: la lista fue creada.
// Post: devuelve un nuevo iterador externo. Si hubo un fallo al 
// crear el iterador externo, devuelve NULL. 
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza a la siguiente posición de la lista. 
// Pre: el iterador fue creado
// Post: devuelve true si se pudo avanzar hacia la posición siguiente. 
// Devuelve false si la posición actual es null
bool lista_iter_avanzar(lista_iter_t *iter);


// Obtiene el valor del elemento en la posición actual del iterador. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía 
// o si el iterador ya llego a la posición final devuelve NULL.
// Pre: el iterador fue creado
// Post: se devolvió el dato del elemento elemento en la posición actual del iterador, cuando la lista no está vacía.
void *lista_iter_ver_actual(const lista_iter_t *iter);


// Comprueba si el iterador ya llegó a la posición final de la lista. 
// Pre: el iterador fue creado
// Post: se devolvió true si llegó al final de la lista, false en caso contrario
bool lista_iter_al_final(const lista_iter_t *iter);


// Destruye el iterador
// Pre: el iterador fue creado
// Post: el iterador fue destruido
void lista_iter_destruir(lista_iter_t *iter);


// Inserta un elemento en la posición actual
// Pre: el iterador fue creado
// Post: se agregó un nuevo elemento a la lista
bool lista_iter_insertar(lista_iter_t *iter, void *dato);


// Elimina un elemento en la posición actual
// Pre: el iterador fue creado
// Post: se borró un elemento a la lista y se devolvió el elemento, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_iter_borrar(lista_iter_t *iter);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
void pruebas_lista_estudiante(void);

#endif  // PILA_H