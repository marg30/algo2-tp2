#include <stdbool.h>
#include <stddef.h>

struct paciente;

typedef struct paciente paciente_t;

//Crea un puntero a paciente. Recibe como parametros el nombre del mismo y el año en el cual se incribiò a la clinica
//Post: La estructura paciente fue inicializada
paciente_t* paciente_crear(char* nombre, int anio_inscripcion);

//Destruye la estructura paciente liberando la memoria pedida
//Pre: La estructura paciente fue inicializada
//Post: La estructura paciente fue destruida
void paciente_destruir(paciente_t* paciente);