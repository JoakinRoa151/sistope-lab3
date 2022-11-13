#include "funciones.h"

extern int chunk;
extern int anio;
extern int precioMenor;
extern FILE *archivoEntrada;
extern pthread_mutex_t lock;

// Descripción: Función que traspasa de un string a struct.
// Entrada: array.
// Salida: array con los juegos por año.
calculosJuegoPorAnio lecturaCalculo(char *lineaCalculo)
{
    char *puntero_string;

    calculosJuegoPorAnio nuevoCalculo;
    puntero_string = strtok(lineaCalculo, ",");
    nuevoCalculo.anioJuego = atoi(puntero_string);
    // Secuencia de llamadas a strtok para colocar "," como separador.
    puntero_string = strtok(NULL, ",");
    nuevoCalculo.cantidadJuegos = atoi(puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoCalculo.windows = atoi(puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoCalculo.mac = atoi(puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoCalculo.linuxx = atoi(puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoCalculo.nombreJuegoMasCaro, puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoCalculo.nombreJuegoMasBarato, puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoCalculo.juegosGratis, puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoCalculo.precioJuegoMasCaro = atof(puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoCalculo.precioJuegoMasBarato = atof(puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoCalculo.sumaPreciosPorAnio = atof(puntero_string);
    return nuevoCalculo;
}





void *funcionHebra(void *arg){
    while (!feof(archivoEntrada)){
        char buffer[2000];
        pthread_mutex_lock(&lock);
        for (int i = 0; i < chunk; i++){
            if (fgets(buffer, 2000, archivoEntrada) == NULL){
                break;
            }
            printf("%s", buffer);
            //juego nuevoJuego = lecturaJuego(buffer);
            
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}