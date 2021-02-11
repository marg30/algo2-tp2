#define _POSIX_C_SOURCE 200809L // strdup
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "pila.h"

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

enum lado_hijo {
    IZQ, 
    DER
};

/* Definición del struct nodo.
 */
typedef struct nodo {
    struct nodo* izq;
    struct nodo* der;
    char* clave;
    void* dato;
} nodo_t;

/* Definición del struct abb.
 */
typedef struct abb {
    nodo_t* raiz;
    size_t cant;
    abb_destruir_dato_t destruir;
    abb_comparar_clave_t comparar;
} abb_t;


// Crea un nodo
nodo_t* crear_nodo(const char* clave, void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    size_t tam_clave = strlen(clave) + 1; // Suma 1 porque strlen no cuenta el \0
    char* clave_copia = malloc(sizeof(char) * tam_clave);
    if(!(clave_copia)) return false;
    strcpy(clave_copia, clave);
    if (!(nodo)) return NULL;
    nodo->dato = dato;
    nodo->clave = clave_copia;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

// Destruye un nodo.
// Pre: el nodo fue creado.
// Post: la memoria del nodo fue liberada.
void *destruir_nodo(nodo_t *nodo) {
    void *dato = nodo->dato;

    free(nodo->clave);
    free(nodo);

    return dato;
}

/* *****************************************************************
 *                    FUNCIONES 
 *                    AUXILIARES
 * *****************************************************************/

nodo_t* buscar_nodo_aux(const abb_t* arbol, nodo_t* nodo, const char* clave, bool guardar, int* lado){
    abb_comparar_clave_t comparar = arbol->comparar;


    if (!nodo) {
        return NULL;
    } 
    
    if (comparar(nodo->clave, clave) < 0) {
        if(guardar && !nodo->der) {
            *lado = DER;
            return nodo;
        }
        return buscar_nodo_aux(arbol, nodo->der, clave, guardar, lado);
    } 
    
    if (comparar(nodo->clave, clave) > 0) {
        if(guardar && !nodo->izq) {
            *lado = IZQ;
            return nodo;
        }
        return buscar_nodo_aux(arbol,nodo->izq, clave, guardar, lado);
    }
    return nodo;
}

nodo_t *buscar_padre(const abb_t* arbol, nodo_t* nodo, const char* clave, int *lado_hijo){
    if (nodo->izq == NULL && nodo->der == NULL) return NULL;

    abb_comparar_clave_t comparar = arbol->comparar;

    if (nodo->izq && comparar(nodo->izq->clave, clave) == 0) {
        *lado_hijo = IZQ;
        return nodo;
    } 
    
    if (nodo->der && comparar(nodo->der->clave, clave) == 0) {
        *lado_hijo = DER;
        return nodo;
    } 
    
    if (comparar(nodo->clave, clave) < 0 && nodo->der != NULL) {
        return buscar_padre(arbol, nodo->der, clave, lado_hijo);
    } 

    return buscar_padre(arbol, nodo->izq, clave, lado_hijo);

}

size_t cantidad_hijos_nodo(nodo_t *nodo) {
    size_t cant = 0;
    if (nodo->der != NULL) cant++;
    if (nodo->izq != NULL) cant++;

    return cant;
}

nodo_t *buscar_reemplazante(nodo_t *nodo) {
    if (!nodo) return NULL;
    if (nodo->der == NULL) return nodo;
    return buscar_reemplazante(nodo->der);
}

char *aux_borrar_nodo_dos_hijos(abb_t *arbol, nodo_t *nodo, nodo_t *padre, int lado) {
    nodo_t *reemplazante = buscar_reemplazante(nodo->izq);
    void* dato_aux = nodo->dato;
    char* clave_reemplazante = strdup(reemplazante->clave);
    char* dato_reemplazante = abb_borrar(arbol, reemplazante->clave);

    free(nodo->clave);
    nodo->clave = clave_reemplazante;
    nodo->dato = dato_reemplazante;
    return dato_aux;
}

void _abb_destruir(nodo_t* nodo, abb_t* arbol){

    if (!nodo) return;

    _abb_destruir(nodo->izq, arbol);
    _abb_destruir(nodo->der, arbol);
    if (arbol->destruir) {
        abb_destruir_dato_t destruir_dato = arbol->destruir;
        destruir_dato(nodo->dato);
    }
    destruir_nodo(nodo);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!(abb)) return NULL;
    abb->raiz = NULL;
    abb->cant = 0;
    abb->destruir = destruir_dato;
    abb->comparar = cmp;
    return abb;
}


bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if (abb_pertenece(arbol, clave)){
        abb_destruir_dato_t destruir = arbol->destruir;
        nodo_t* nodo_a_modificar = buscar_nodo_aux(arbol, arbol->raiz, clave, false, NULL);
        if(destruir){
            destruir(nodo_a_modificar->dato);
        }
        nodo_a_modificar->dato = dato;

    } else {
        nodo_t* nodo_nuevo = crear_nodo(clave, dato);

        if (!arbol->raiz) {
            arbol->raiz = nodo_nuevo;
            if(!arbol->raiz) return false;
        } 
        else {
            int lado;
            nodo_t* nodo_padre = buscar_nodo_aux(arbol, arbol->raiz, clave, true, &lado);
            if (!nodo_padre) return false;
            if (lado == IZQ) nodo_padre->izq = nodo_nuevo;
            else nodo_padre->der = nodo_nuevo;
        }
        arbol->cant++;
    }
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (abb_cantidad(arbol) == 0 || !abb_pertenece(arbol, clave)){
        return NULL;
    } 

    int lado;
    nodo_t *padre = NULL; 
    abb_comparar_clave_t comparar = arbol->comparar;
    bool es_raiz = false;
    nodo_t *nodo_a_borrar = NULL;
    

    if (comparar(clave, arbol->raiz->clave) == 0){
        nodo_a_borrar = arbol->raiz;
        es_raiz = true;
    } else {
        padre = buscar_padre(arbol, arbol->raiz, clave, &lado);
        if (!padre) return NULL;
        if (lado == IZQ) nodo_a_borrar = padre->izq;
        else nodo_a_borrar = padre->der;
    }

    if (!nodo_a_borrar) return NULL;

    size_t cant_hijos = cantidad_hijos_nodo(nodo_a_borrar);

    if (cant_hijos == 0){
        if (es_raiz) arbol->raiz = NULL;
        else if (lado == IZQ) padre->izq = NULL;
        else if (lado == DER) padre->der = NULL;
        arbol->cant--;
        return destruir_nodo(nodo_a_borrar);

    } else if (cant_hijos == 1) {
        nodo_t *nodo_hijo;
        if (nodo_a_borrar->izq != NULL) {
            nodo_hijo = nodo_a_borrar->izq;
        } else {
            nodo_hijo = nodo_a_borrar->der;
        }
        if (!padre) arbol->raiz = nodo_hijo;
        else if (lado == IZQ) padre->izq = nodo_hijo;
        else padre->der = nodo_hijo;
        arbol->cant--; 
        return destruir_nodo(nodo_a_borrar);
    } 
    return aux_borrar_nodo_dos_hijos(arbol, nodo_a_borrar, padre, lado);
}

void* abb_obtener(const abb_t* arbol, const char* clave){
    nodo_t* nodo = buscar_nodo_aux(arbol, arbol->raiz, clave, false, NULL);
    if (!nodo) return NULL;
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    return buscar_nodo_aux(arbol, arbol->raiz, clave, false, NULL) ? true: false;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void abb_destruir(abb_t* arbol){
    _abb_destruir(arbol->raiz, arbol);    
    free(arbol);
}

/* *****************************************************************
 *                    FUNCIONES DEL ITERADOR
 *                    INTERNO
 * *****************************************************************/

bool abb_iterar(nodo_t *nodo, bool visitar(const char *, void *, void *, void *, void *), void *extra, void *limite_1, void *limite_2) {
    if (!nodo) return true;
    if(!abb_iterar(nodo->izq, visitar, extra, limite_1, limite_2)){
        return false;
    }
    if (!visitar(nodo->clave, nodo->dato, extra, limite_1, limite_2)) {
        return false;
    }
    if (!abb_iterar(nodo->der, visitar, extra, limite_1, limite_2)) {
        return false;
    }
    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *, void *, void *), void *extra, void *limite_1, void *limite_2){
    abb_iterar(arbol->raiz, visitar, extra, limite_1, limite_2);
}


/* Definición del struct abb_iter
 */
typedef struct abb_iter {
    const abb_t* abb;
    pila_t* pila_recorrido;
} abb_iter_t;


/* *****************************************************************
 *                    FUNCIONES DEL ITERADOR
 *                    EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){

    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!(iter)) return NULL;
    iter->abb = arbol;
    iter->pila_recorrido = pila_crear();

    if(!(iter->pila_recorrido)){
        free(iter);
        return NULL;
    }

    nodo_t* nodo_actual = arbol->raiz;
    while(nodo_actual){
        pila_apilar(iter->pila_recorrido, nodo_actual);
        nodo_actual = nodo_actual->izq;
    }
    return iter;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila_recorrido);
}

bool abb_iter_in_avanzar(abb_iter_t *iter){

    if(abb_iter_in_al_final(iter)) return false;
    nodo_t* desapilado = (nodo_t*)pila_desapilar(iter->pila_recorrido);
    if(desapilado->der){
        nodo_t* prox_nodo = desapilado->der;
        while(prox_nodo){
            pila_apilar(iter->pila_recorrido, prox_nodo);
            prox_nodo = prox_nodo->izq;
        }
    }

    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return NULL;
    nodo_t* nodo_actual = (nodo_t*)pila_ver_tope(iter->pila_recorrido);
    return nodo_actual->clave;
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila_recorrido);
    free(iter);
}