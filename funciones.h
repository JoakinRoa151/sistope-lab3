#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>
#include "estructura.h"

void* funcionHebra(void* arg);
calculosJuegoPorAnio lecturaCalculo(char *lineaCalculo);
calculosJuegoPorAnio *agregarCalculoLista(calculosJuegoPorAnio *listaCalculos, calculosJuegoPorAnio calculoAgregar, int *n);
juego lecturaJuego(char *lineaJuego);
bool verificarAnio(calculosJuegoPorAnio * listadoCalculos, juego nuevoJuego, int n);
int buscarPosicionCalculo(calculosJuegoPorAnio*listaCalculos, int anio, int n);
void compararCalculos(calculosJuegoPorAnio *  listadoCalculosPorAnio, juego nuevoJuego, int tamanioCalculos);
void merge(calculosJuegoPorAnio* calculos,int inicio,int medio,int fin);
void mergeSort(calculosJuegoPorAnio* calculos, int inicio, int fin);