#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

void pruebas_heap_catedra(void);
void pruebas_heap_volumen_catedra(size_t, bool);

// **********************************************************************************
//                         PROGRAMA PRINCIPAL
typedef int (*cmp_func_t) (const void *a, const void *b);

typedef struct heap{
    void** datos;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t comparar;
} heap_t;

//***********************************************************************************
//                    FUNCIONES AUXILIARES

void swap(void** x, void** y){
    void* aux = *x;
    *x = *y;
    *y = aux;
}

void upheap(void** arreglo, size_t hijo, cmp_func_t cmp){
    if (hijo == 0) return;
    size_t padre = (hijo - 1) / 2;
    if(cmp(arreglo[hijo], arreglo[padre]) > 0){
		swap(&arreglo[hijo], &arreglo[padre]);
        upheap(arreglo, padre, cmp);
    }
}

void downheap(void** arreglo, size_t tam, size_t padre, cmp_func_t cmp){
    if(padre == tam - 1) return;
    size_t hijo_izq = (2 * padre) + 1;
    size_t hijo_der = (2 * padre) + 2;
    if(hijo_izq < tam && hijo_der < tam){
        if(!(cmp(arreglo[padre], arreglo[hijo_izq]) >= 0 && cmp(arreglo[padre], arreglo[hijo_der]) >= 0)){
            size_t mayor = cmp(arreglo[hijo_izq], arreglo[hijo_der]) > 0 ? hijo_izq : hijo_der;
			swap(&arreglo[padre], &arreglo[mayor]);   
            downheap(arreglo, tam, mayor, cmp);
        }
    }else if(hijo_izq < tam){
        if(!(cmp(arreglo[padre], arreglo[hijo_izq]) >= 0)){
			swap(&arreglo[padre], &arreglo[hijo_izq]);
        }
    }
}

void heapify(void* arreglo[], size_t tam, cmp_func_t cmp){
    for(size_t i = tam / 2; i > 0; i--){
        downheap(arreglo, tam, i - 1, cmp);
    }
}

bool heap_redimensionar(heap_t *heap, bool agrandar){
    if (agrandar){
        heap->capacidad *= 2;
    }else{
        heap->capacidad /= 2;
    }
    void **datos_nuevo = realloc(heap->datos, heap->capacidad * sizeof(void*));
    if (!(datos_nuevo)) return false;
    heap->datos = datos_nuevo;
    return true;
}

//***********************************************************************************
//                   FUNCIONES PRINCIPALES

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if (!(heap)) return NULL;
    heap->datos = malloc(10 * sizeof(void*));
    if(!(heap->datos)){
        free(heap);
        return NULL;
    }
    heap->cantidad = 0;
    heap->capacidad = 10;
    heap->comparar = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if (!(heap)) return NULL;
    heap->datos = malloc(n * sizeof(void*));
    if(!(heap->datos)){
        free(heap);
        return NULL;
    }
    for(size_t i = 0; i < n; i++){
        heap->datos[i] = arreglo[i];
    }
    heapify(heap->datos, n, cmp);
    heap->cantidad = n;
    heap->capacidad = n > 10 ? n * 2 : 10;
    heap->comparar = cmp;
    return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
    if(destruir_elemento){
        for(int i = 0; i < heap->cantidad; i++){
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap->cantidad == 0 ? true : false;
}

bool heap_encolar(heap_t *heap, void *elem){
    if (heap->cantidad == heap->capacidad){
        if(!(heap_redimensionar(heap, true))) return false;
    }
    heap->datos[heap->cantidad] = elem;
    upheap(heap->datos, heap->cantidad, heap->comparar);
    heap->cantidad++;
    return true;
}

void *heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    return heap->datos[0];
}

void *heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    heap->cantidad--;
    if((heap->cantidad)*4 <= heap->capacidad && heap->capacidad / 2 >= 10){
        if(!(heap_redimensionar(heap, false))) return NULL;
    }
    void* elem = heap->datos[0];
    heap->datos[0] = heap->datos[heap->cantidad];
    heap->datos[heap->cantidad] = NULL;
    if(heap->cantidad > 1){
        downheap(heap->datos, heap->cantidad, 0, heap->comparar);
    }
    return elem;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos, cant, cmp);
    while(cant > 0){
		swap(&elementos[0], &elementos[cant - 1]);
        cant--;
        downheap(elementos, cant, 0, cmp);
    }
}