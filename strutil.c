#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char *substr(const char *str, size_t n){
    char* nuevo_str;
    size_t len = strlen(str);
    if (n > len){
        nuevo_str = strdup(str);
    }
    else {
        nuevo_str = strndup(str, n);
    }

    if (nuevo_str == NULL){
        return NULL;
    }

    return nuevo_str;
}

//Recibe una cadena, un separador y el largo de la cadena
//Devuelve la cantidad de separadores que hay en la cadena
int contar_separadores(const char *str, char sep, size_t len){
    int contador = 0;
    for (size_t i = 0; i < len; i++){
        if (str[i] == sep){
            contador++;
        }
    }
    return contador;
}


//Recibe un arreglo de arreglos
//Devuelve el largo del arreglo
size_t calc_len_string_en_array(char** strv) {

	size_t len = 0;
    size_t i = 0;

	while (strv[i] != NULL){
		len += strlen(strv[i]) + 2;
        i++;
	}

	return len;
}


char **split(const char *str, char sep){
    size_t len = strlen(str);
    int separadores = contar_separadores(str, sep, len);
    separadores += 2;
    char **strv = malloc(separadores * sizeof(char*));
    size_t posicion = 0;
    size_t inicio = 0;

    char *cadena_vacia = "";


    if (strv == NULL){
        free(strv);
        return NULL;
    }
  
    if(strcmp(str,cadena_vacia) == 0){
        strv[0] = strdup(cadena_vacia);
        strv[1] = NULL;
        return strv;
    }

    if(len == 1 && str[0] == sep){
        strv[0] = strdup(cadena_vacia);
        strv[1] = strdup(cadena_vacia);
        strv[2] = NULL;
        return strv;
    }

    for(size_t i = 0; i < len; i++){
        if(str[i] == sep){
            strv[posicion] = substr(str + inicio, i - inicio);
            inicio = i + 1;
            posicion++;
        }
        if(i == len - 1){
            strv[posicion] = substr(str + inicio, i + 1 - inicio);
            posicion++;
        }
    }

    if(str[len - 1] == sep){
        strv[posicion] = "";
    }
    strv[posicion] = NULL;

    return strv;
}

char *join(char **strv, char sep){
    char cero = '\0';
    char* resultado;
    char* puntero_a_final;

    if(strv == NULL || strv[0] == NULL){
        resultado = strdup(&cero);
        return resultado;
    }
    char sepp[2] = {sep, cero}; 
    size_t cant_caract = calc_len_string_en_array(strv);
    if (cant_caract == 0) {
        cant_caract++;
    }
    resultado = malloc(cant_caract* sizeof(char*));
    int i = 1;
    if (resultado == NULL){
        return resultado;
    }
    resultado[0] = '\0';
    puntero_a_final = stpcpy(resultado, strv[0]);
    puntero_a_final = stpcpy(puntero_a_final, sepp);

    
    while (strv[i] != NULL) {
        puntero_a_final = stpcpy(puntero_a_final, strv[i]);
        puntero_a_final = stpcpy(puntero_a_final, sepp);
        i++;
    }
    if(sep != cero && strlen(resultado) > 0){
        puntero_a_final = stpcpy(puntero_a_final - 1, &cero);
    }

    return resultado;

}

void free_strv(char *strv[]) {
    size_t i = 0;
    if (strv == NULL){
        return;
    }
    while(strv[i] != NULL){
        free(strv[i]);
        i++;
    }
    i++;
    free(strv);
}