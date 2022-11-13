#ifndef estructuras_h_
#define estructuras_h_
#include <stdio.h>
// estructura para guardar la información pertinente de un juego.
typedef struct juego juego;
struct juego{
    int id;
    char nombre [100] ;
    int restriccion;
    float precio;
    char proximamente [100];
    int fechaLanzamiento;
    char gratuidad [100];
    char windows [100];
    char mac [100];
    char linuxx [100];
    };

// Se crea una estrcutura para guardar los datos de los juegos por año.
typedef struct calculosJuegoPorAnio calculosJuegoPorAnio;
struct calculosJuegoPorAnio{
    int anioJuego;
    int cantidadJuegos;
    char nombreJuegoMasCaro[100];
    float precioJuegoMasCaro;
    float precioJuegoMasBarato;
    char nombreJuegoMasBarato[100];
    int windows;
    int linuxx;
    int mac;
    float sumaPreciosPorAnio;
    char juegosGratis[2500];
    };
#endif /* estructuras_h_ */