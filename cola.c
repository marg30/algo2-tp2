#include "cola.h"
#include <stdlib.h>

/* Definición del struct cola.
 */

typedef struct nodo nodo_t;

struct nodo {
    void* dato;
    nodo_t* sig;
};


struct cola {
    nodo_t *primero;
    nodo_t *ultimo;
};

static nodo_t *nodo_crear(void *valor){
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL){
        return NULL;
    }
    nodo->dato = valor;
    nodo->sig = NULL;
    return nodo;
} 

// Destruye un nodo.
// Pre: el nodo fue creado.
// Post: la memoria del nodo fue liberada.
void nodo_destruir(nodo_t* nodo) {
 	free(nodo);
}


cola_t *cola_crear(void) {
    cola_t *cola = malloc(sizeof(cola_t));

    if (cola == NULL) {
        return NULL;
    }
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
	while (!cola_esta_vacia(cola)){
		void* dato = cola_desencolar(cola);
    	if (destruir_dato){
            destruir_dato(dato);
        }
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
    return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){
    nodo_t *nodo = nodo_crear(valor);
    if (nodo == NULL) {
        return false;
    }
    if (cola->primero == NULL){
        cola->primero = nodo;
        cola->ultimo = nodo;

    } else {
        nodo_t *ultimo = cola->ultimo;
        ultimo->sig = nodo;
        cola->ultimo = nodo;
    }
    return true;
}

void *cola_ver_primero(const cola_t *cola){
    if (cola_esta_vacia(cola)){
        return NULL;
    }
    return cola->primero->dato;
}

void *cola_desencolar(cola_t *cola){
    if (cola_esta_vacia(cola)){
        return NULL;
    }
    nodo_t *primero = cola->primero;
    cola->primero = cola->primero->sig;
    if (!cola->primero){
        cola->ultimo = NULL;
    }
    void* valor = primero->dato;
    nodo_destruir(primero);

    return valor;
}