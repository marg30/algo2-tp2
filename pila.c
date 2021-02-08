#include "pila.h"

#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

#define CAPACIDAD_INICIAL 10

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));

    if (pila == NULL) {
        return NULL;
    }

    pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->cantidad = 0;

    return pila;
}


void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){

    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){
    if (pila == NULL){
        return false;
    }
    pila->datos[pila->cantidad ++] = valor;
    
    if (pila->cantidad == pila->capacidad) {
        size_t capacidad_nueva = pila->capacidad*2;
        void **datos_nuevo = realloc(pila->datos, capacidad_nueva * sizeof(void*));
        if (capacidad_nueva > 0 && datos_nuevo == NULL) {
            return false;
        }
        pila->datos = datos_nuevo;
        pila->capacidad = capacidad_nueva;
    }

    return true;

}

void *pila_ver_tope(const pila_t *pila){
    if (pila == NULL || pila_esta_vacia(pila)) {
        return NULL;
    }
    return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila){

    void *elem = pila_ver_tope(pila);

    if (pila == NULL) {
        return elem;
    }

    if ((pila->cantidad - 1)*4 <= pila->capacidad) {
        size_t capacidad_nueva = pila->capacidad/2;
        void **datos_nuevo = realloc(pila->datos, capacidad_nueva * sizeof(void*));

        if (capacidad_nueva > 0 && datos_nuevo == NULL) {
            return false;
        }

        pila->datos = datos_nuevo;
        pila->capacidad = capacidad_nueva;
    }

    if (pila->cantidad > 0) {
        pila->cantidad--;
    }
    else {
        pila->capacidad = CAPACIDAD_INICIAL;
    }

    return elem;
}