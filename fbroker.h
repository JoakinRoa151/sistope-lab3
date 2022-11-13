#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include "estructura.h"

calculosJuegoPorAnio *agregarCalculoLista(calculosJuegoPorAnio *listaCalculos, calculosJuegoPorAnio calculoAgregar, int *n);
calculosJuegoPorAnio lecturaCalculo(char *lineaJuego);
int verificarCalculoEnLista(calculosJuegoPorAnio * listaCalculos,calculosJuegoPorAnio calculo,int n);
calculosJuegoPorAnio * evaluacionCalculo(calculosJuegoPorAnio * listaCalculo, calculosJuegoPorAnio calculoPorAnio, int * banderas, int * listaAnios, int cantidadAniosTotales);
int *aniosEnLista(calculosJuegoPorAnio ** lista, int n, int *tamanio, int *elem);
void merge(calculosJuegoPorAnio* calculos,int inicio,int medio,int fin);
void mergeSort(calculosJuegoPorAnio* calculos, int inicio, int fin);