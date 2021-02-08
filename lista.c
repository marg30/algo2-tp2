#include "lista.h"
#include <stdlib.h>

/* Definición del struct nodo.
 */

typedef struct nodo {
    void* dato;
    struct nodo* prox;
} nodo_t;


/* Definición del struct lista.
 */

typedef struct lista {
    struct nodo* prim;
    struct nodo* ult;
    size_t largo;
} lista_t;


/* Definición del struct lista_iter.
 */
typedef struct lista_iter {
    struct nodo* act;
    struct nodo* ant;
    struct lista* lista;
} lista_iter_t;


static nodo_t *nodo_crear(void *valor){
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL){
        return NULL;
    }
    nodo->dato = valor;
    nodo->prox = NULL;
    return nodo;
} 

// Destruye un nodo.
// Pre: el nodo fue creado.
// Post: la memoria del nodo fue liberada.
static void nodo_destruir(nodo_t* nodo) {
 	free(nodo);
}

lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));

    if (lista == NULL) {
        return NULL;
    }
    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;
    return lista;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
	while (!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
    	if (destruir_dato){
            destruir_dato(dato);
        }
    }
    free(lista);
}

bool lista_esta_vacia(const lista_t *lista){
    return lista->prim == NULL || lista->largo == 0;
}

bool lista_insertar_ultimo(lista_t *lista, void *valor){
    nodo_t *nodo = nodo_crear(valor);
    if (nodo == NULL) {
        return false;
    }
    if (lista->prim == NULL){
        lista->prim = nodo;

    } else {
        nodo_t *ult = lista->ult;
        ult->prox = nodo;
    }
    lista->ult = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_primero(lista_t *lista, void *valor){
    if (lista == NULL){
        return false;
    }
    nodo_t *nodo = nodo_crear(valor);
    if (nodo == NULL) {
        return false;
    }
    if (lista->prim == NULL){
        lista->ult = nodo;

    } else {
        nodo->prox = lista->prim;
    }
    lista->prim = nodo;
    lista->largo++;
    return true;
}

void *lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t *lista){
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    return lista->ult->dato;
}

void *lista_borrar_primero(lista_t *lista){
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    nodo_t *prim = lista->prim;
    lista->prim = lista->prim->prox;
    if (!lista->prim){
        lista->ult = NULL;
    }
    void* valor = prim->dato;
    nodo_destruir(prim);
    lista->largo--;
    return valor;
}

size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t *iter = malloc(sizeof(lista_iter_t));

    if (iter == NULL) {
        return NULL;
    }
    iter->ant = NULL;
    iter->act = lista->prim;
    iter->lista = lista;
    return iter;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return iter->act == NULL;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if (lista_iter_al_final(iter)){
        return false;
    }
    iter->ant = iter->act;
    iter->act = iter->act->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    if (iter == NULL ||lista_iter_al_final(iter)){
        return NULL;
    }
    return iter->act->dato;
}


bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t *nodo = nodo_crear(dato);
    if (nodo == NULL){
        return false;
    }
    if(iter->ant == NULL){
        iter->lista->prim = nodo;
    }
    else {
        iter->ant->prox = nodo;
    }

    if (lista_iter_al_final(iter)) {
        iter->lista->ult = nodo;
    }

    nodo->prox = iter->act;
    iter->act = nodo;
    iter->lista->largo++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
    if (iter == NULL || lista_iter_al_final(iter)){
        return NULL;
    }
    nodo_t *a_eliminar = iter->act;
    if (iter->ant == NULL){
        iter->lista->prim = iter->act->prox;
    }
    else{
        iter->ant->prox = a_eliminar->prox;
    }

    if (a_eliminar->prox == NULL){
        iter->lista->ult = iter->ant;
    }
    iter->act = a_eliminar->prox;
    void *dato_a_eliminar = a_eliminar->dato;
    nodo_destruir(a_eliminar);
    iter->lista->largo--;

    return dato_a_eliminar;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    if (lista_esta_vacia(lista) ){
        return;
    }

    nodo_t *act = lista->prim;
    bool iterar = true;
    while(act != NULL && iterar){
        iterar = visitar(act->dato, extra);
        act = act->prox;
    }
}