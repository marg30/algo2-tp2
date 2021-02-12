#include <stdbool.h>
#include <stddef.h>

struct paciente;

typedef struct paciente paciente_t;

//Compara el anio de inscripcion de dos pacientes. Devuelve 1 si el paciente 1 se inscribio antes, -1 si lo hizo despues, 
//o 0 si lo hizo en el mismo anio que el paciente 2
int cmp_pacientes(const paciente_t* paciente_1, const paciente_t* paciente_2);

//Crea un puntero a paciente. Recibe como parametros el nombre del mismo y el año en el cual se incribiò a la clinica
//Post: La estructura paciente fue inicializada
paciente_t* paciente_crear(char* nombre, int anio_inscripcion);

//Devuelve el anio de inscripcion del paciente
//Pre: La estructura paciente fue inicializada
int paciente_ver_anio_insc(paciente_t* paciente);

char *paciente_obtener_nombre(paciente_t* paciente);

//Destruye la estructura paciente liberando la memoria pedida
//Pre: La estructura paciente fue inicializada
//Post: La estructura paciente fue destruida
void paciente_destruir(paciente_t* paciente);

