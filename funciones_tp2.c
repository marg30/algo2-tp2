#include <stdio.h>
#include "doctor.h"
#include "especialidades.h"
#include "testing.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

int leer_archivo_doctores(){
    FILE* archivo = fopen("doctores.csv", "r");
    if (archivo == -1) {
        return -1;
    }
    procesar_entrada();

}