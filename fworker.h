#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include "estructura.h"

juego lecturaJuego(char *lineaJuego);
juego *agregarJuegoLista(juego *lista, juego juegoAgregar, int *n);
void merge(juego* listadoJuegos,int inicio,int medio,int fin);
void mergeSort(juego* listadoJuegos, int inicio, int fin);
calculosJuegoPorAnio *agregarCalculoLista(calculosJuegoPorAnio *listaCalculos, calculosJuegoPorAnio calculoAgregar, int *n);