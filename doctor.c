#define _POSIX_C_SOURCE 200809L // strdup
#include "doctor.h"
#include "especialidades.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "pila.h"

typedef int (*doctores_comparar_nombre_t) (const char *, const char *);
typedef void (*doctores_destruir_dato_t) (void *);

enum lado_hijo {
    IZQ, 
    DER
};

/* Definición del struct doctor.
 */
typedef struct doctor {
    struct doctor* izq;
    struct doctor* der;
    char* nombre;
    void* dato;
    especialidades_t* especialidad;
    int pacientes_atendidos;
} doctor_t;

/* Definición del struct doctores.
 */
typedef struct doctores {
    doctor_t* raiz;
    size_t cant;
    doctores_destruir_dato_t destruir;
    doctores_comparar_nombre_t comparar;
} doctores_t;


// Crea un doctor
doctor_t* crear_doctor(const char* clave, void* dato){
    doctor_t* doctor = malloc(sizeof(doctor_t));
    size_t tam_clave = strlen(clave) + 1; // Suma 1 porque strlen no cuenta el \0
    char* clave_copia = malloc(sizeof(char) * tam_clave);
    if(!(clave_copia)) return false;
    strcpy(clave_copia, clave);
    if (!(doctor)) return NULL;
    doctor->dato = dato;
    doctor->nombre = clave_copia;
    doctor->izq = NULL;
    doctor->der = NULL;
    return doctor;
}

// Destruye un doctor.
// Pre: el doctor fue creado.
// Post: la memoria del doctor fue liberada.
void *destruir_doctor(doctor_t *doctor) {
    void *dato = doctor->dato;

    free(doctor->nombre);
    free(doctor);

    return dato;
}

/* *****************************************************************
 *                    FUNCIONES 
 *                    AUXILIARES
 * *****************************************************************/

doctor_t* buscar_doctor_aux(const doctores_t* doctores, doctor_t* doctor, const char* clave, bool guardar, int* lado){
    doctores_comparar_nombre_t comparar = doctores->comparar;


    if (!doctor) {
        return NULL;
    } 
    
    if (comparar(doctor->nombre, clave) < 0) {
        if(guardar && !doctor->der) {
            *lado = DER;
            return doctor;
        }
        return buscar_doctor_aux(doctores, doctor->der, clave, guardar, lado);
    } 
    
    if (comparar(doctor->nombre, clave) > 0) {
        if(guardar && !doctor->izq) {
            *lado = IZQ;
            return doctor;
        }
        return buscar_doctor_aux(doctores,doctor->izq, clave, guardar, lado);
    }
    return doctor;
}

doctor_t *buscar_padre(const doctores_t* doctores, doctor_t* doctor, const char* clave, int *lado_hijo){
    if (doctor->izq == NULL && doctor->der == NULL) return NULL;

    doctores_comparar_nombre_t comparar = doctores->comparar;

    if (doctor->izq && comparar(doctor->izq->nombre, clave) == 0) {
        *lado_hijo = IZQ;
        return doctor;
    } 
    
    if (doctor->der && comparar(doctor->der->nombre, clave) == 0) {
        *lado_hijo = DER;
        return doctor;
    } 
    
    if (comparar(doctor->nombre, clave) < 0 && doctor->der != NULL) {
        return buscar_padre(doctores, doctor->der, clave, lado_hijo);
    } 

    return buscar_padre(doctores, doctor->izq, clave, lado_hijo);

}

size_t cantidad_hijos_doctor(doctor_t *doctor) {
    size_t cant = 0;
    if (doctor->der != NULL) cant++;
    if (doctor->izq != NULL) cant++;

    return cant;
}

doctor_t *buscar_reemplazante(doctor_t *doctor) {
    if (!doctor) return NULL;
    if (doctor->der == NULL) return doctor;
    return buscar_reemplazante(doctor->der);
}

char *aux_borrar_doctor_dos_hijos(doctores_t *doctores, doctor_t *doctor, doctor_t *padre, int lado) {
    doctor_t *reemplazante = buscar_reemplazante(doctor->izq);
    void* dato_aux = doctor->dato;
    char* nombre_reemplazante = strdup(reemplazante->nombre);
    char* dato_reemplazante = doctores_borrar(doctores, reemplazante->nombre);

    free(doctor->nombre);
    doctor->nombre = nombre_reemplazante;
    doctor->dato = dato_reemplazante;
    return dato_aux;
}

void _doctores_destruir(doctor_t* doctor, doctores_t* doctores){

    if (!doctor) return;

    _doctores_destruir(doctor->izq, doctores);
    _doctores_destruir(doctor->der, doctores);
    if (doctores->destruir) {
        doctores_destruir_dato_t destruir_dato = doctores->destruir;
        destruir_dato(doctor->dato);
    }
    destruir_doctor(doctor);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/
doctores_t* doctores_crear(doctores_comparar_nombre_t cmp, doctores_destruir_dato_t destruir_dato){
    doctores_t* doctores = malloc(sizeof(doctores_t));
    if(!(doctores)) return NULL;
    doctores->raiz = NULL;
    doctores->cant = 0;
    doctores->destruir = destruir_dato;
    doctores->comparar = cmp;
    return doctores;
}


bool doctores_guardar(doctores_t *doctores, const char *nombre, void *dato){
    if (doctores_pertenece(doctores, nombre)){
        doctores_destruir_dato_t destruir = doctores->destruir;
        doctor_t* doctor_a_modificar = buscar_doctor_aux(doctores, doctores->raiz, nombre, false, NULL);
        if(destruir){
            destruir(doctor_a_modificar->dato);
        }
        doctor_a_modificar->dato = dato;

    } else {
        doctor_t* doctor_nuevo = crear_doctor(nombre, dato);

        if (!doctores->raiz) {
            doctores->raiz = doctor_nuevo;
            if(!doctores->raiz) return false;
        } 
        else {
            int lado;
            doctor_t* doctor_padre = buscar_doctor_aux(doctores, doctores->raiz, nombre, true, &lado);
            if (!doctor_padre) return false;
            if (lado == IZQ) doctor_padre->izq = doctor_nuevo;
            else doctor_padre->der = doctor_nuevo;
        }
        doctores->cant++;
    }
    return true;
}

void *doctores_borrar(doctores_t *doctores, const char *nombre){
    if (doctores_cantidad(doctores) == 0 || !doctores_pertenece(doctores, nombre)){
        return NULL;
    } 

    int lado;
    doctor_t *padre = NULL; 
    doctores_comparar_nombre_t comparar = doctores->comparar;
    bool es_raiz = false;
    doctor_t *doctor_a_borrar = NULL;
    

    if (comparar(nombre, doctores->raiz->nombre) == 0){
        doctor_a_borrar = doctores->raiz;
        es_raiz = true;
    } else {
        padre = buscar_padre(doctores, doctores->raiz, nombre, &lado);
        if (!padre) return NULL;
        if (lado == IZQ) doctor_a_borrar = padre->izq;
        else doctor_a_borrar = padre->der;
    }

    if (!doctor_a_borrar) return NULL;

    size_t cant_hijos = cantidad_hijos_doctor(doctor_a_borrar);

    if (cant_hijos == 0){
        if (es_raiz) doctores->raiz = NULL;
        else if (lado == IZQ) padre->izq = NULL;
        else if (lado == DER) padre->der = NULL;
        doctores->cant--;
        return destruir_doctor(doctor_a_borrar);

    } else if (cant_hijos == 1) {
        doctor_t *doctor_hijo;
        if (doctor_a_borrar->izq != NULL) {
            doctor_hijo = doctor_a_borrar->izq;
        } else {
            doctor_hijo = doctor_a_borrar->der;
        }
        if (!padre) doctores->raiz = doctor_hijo;
        else if (lado == IZQ) padre->izq = doctor_hijo;
        else padre->der = doctor_hijo;
        doctores->cant--; 
        return destruir_doctor(doctor_a_borrar);
    } 
    return aux_borrar_doctor_dos_hijos(doctores, doctor_a_borrar, padre, lado);
}

int doctores_obtener_pacientes_atendidos(const doctores_t* doctores, const char* nombre){
    doctor_t* doctor = buscar_doctor_aux(doctores, doctores->raiz, nombre, false, NULL);
    if (!doctor) return NULL;
    return doctor->pacientes_atendidos;
}

especialidades_t* doctores_obtener_especialidad(const doctores_t* doctores, const char* nombre){
    doctor_t* doctor = buscar_doctor_aux(doctores, doctores->raiz, nombre, false, NULL);
    if (!doctor) return NULL;
    return doctor->especialidad; // Habria que ver si devolvemos el nombre u otra cosa
}

// Tal vez las 2 funciones de arriba se pueden combinar en una sola, que reciba por parametro el tipo de dato, la especialidad o los pacientes??

bool doctores_pertenece(const doctores_t *doctores, const char *clave){
    return buscar_doctor_aux(doctores, doctores->raiz, clave, false, NULL) ? true: false;
}

size_t doctores_cantidad(doctores_t *doctores){
    return doctores->cant;
}

void doctores_destruir(doctores_t* doctores){
    _doctores_destruir(doctores->raiz, doctores);    
    free(doctores);
}

/* *****************************************************************
 *                    FUNCIONES DEL ITERADOR
 *                    INTERNO
 * *****************************************************************/

bool doctores_iterar(doctor_t *doctor, bool visitar(const char *, void *, void *), void *extra) {
    if (!doctor) return true;
    if(!doctores_iterar(doctor->izq, visitar, extra)){
        return false;
    }
    if (!visitar(doctor->nombre, doctor->dato, extra)) {
        return false;
    }
    if (!doctores_iterar(doctor->der, visitar, extra)) {
        return false;
    }
    return true;
}

void doctores_in_order(doctores_t *doctores, bool visitar(const char *, void *, void *), void *extra){
    doctores_iterar(doctores->raiz, visitar, extra);
}


/* Definición del struct doctores_iter
 */
typedef struct doctores_iter {
    const doctores_t* doctores;
    pila_t* pila_recorrido;
} doctores_iter_t;


/* *****************************************************************
 *                    FUNCIONES DEL ITERADOR
 *                    EXTERNO
 * *****************************************************************/

doctores_iter_t *doctores_iter_in_crear(const doctores_t *doctores){

    doctores_iter_t* iter = malloc(sizeof(doctores_iter_t));
    if(!(iter)) return NULL;
    iter->doctores = doctores;
    iter->pila_recorrido = pila_crear();

    if(!(iter->pila_recorrido)){
        free(iter);
        return NULL;
    }

    doctor_t* doctor_actual = doctores->raiz;
    while(doctor_actual){
        pila_apilar(iter->pila_recorrido, doctor_actual);
        doctor_actual = doctor_actual->izq;
    }
    return iter;
}

bool doctores_iter_in_al_final(const doctores_iter_t *iter){
    return pila_esta_vacia(iter->pila_recorrido);
}

bool doctores_iter_in_avanzar(doctores_iter_t *iter){

    if(doctores_iter_in_al_final(iter)) return false;
    doctor_t* desapilado = (doctor_t*)pila_desapilar(iter->pila_recorrido);
    if(desapilado->der){
        doctor_t* prox_doctor = desapilado->der;
        while(prox_doctor){
            pila_apilar(iter->pila_recorrido, prox_doctor);
            prox_doctor = prox_doctor->izq;
        }
    }

    return true;
}

const char *doctores_iter_in_ver_actual(const doctores_iter_t *iter){
    if(doctores_iter_in_al_final(iter)) return NULL;
    doctor_t* doctor_actual = (doctor_t*)pila_ver_tope(iter->pila_recorrido);
    return doctor_actual->nombre;
}

void doctores_iter_in_destruir(doctores_iter_t* iter){
    pila_destruir(iter->pila_recorrido);
    free(iter);
}