#include "fbroker.h"

/*
Entrada: lista de struct anioPosicion, posicion final, anio, tamaño de la lista
Salida: lista de struct anioPosicion con un dato agregado
Descripción: funcion que agrega datos de una lista tipo struct anio Posicion
*/
calculosJuegoPorAnio *agregarCalculoLista(calculosJuegoPorAnio *listaCalculos, calculosJuegoPorAnio calculoAgregar, int *n)
{
    calculosJuegoPorAnio *nuevaLista = malloc(sizeof(calculosJuegoPorAnio) * (*n + 1));
    for (int i = 0; i < *n; i++)
    {
        nuevaLista[i]=listaCalculos[i];
    }
    nuevaLista[*n]=calculoAgregar;
    *n = *n + 1;
    free(listaCalculos);
    return nuevaLista;
}

calculosJuegoPorAnio lecturaCalculo(char *lineaCalculo)
{
    char *puntero_string;
    
    calculosJuegoPorAnio nuevoCalculo;
    puntero_string = strtok(lineaCalculo, "+");
    nuevoCalculo.anioJuego = atoi(puntero_string);
    printf("%d\n",nuevoCalculo.anioJuego);
    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.cantidadJuegos = atoi(puntero_string);
    printf("%d\n",nuevoCalculo.cantidadJuegos);
    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.windows = atoi(puntero_string);
    printf("%d\n",nuevoCalculo.windows);
    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.mac = atoi(puntero_string);
    printf("%d\n",nuevoCalculo.mac);
    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.linuxx = atoi(puntero_string);
    printf("%d\n",nuevoCalculo.linuxx);
   
    puntero_string = strtok(NULL, "+");
    strcpy(nuevoCalculo.nombreJuegoMasCaro, puntero_string);
    printf("%s\n",nuevoCalculo.nombreJuegoMasCaro);
    
    puntero_string = strtok(NULL, "+");
    strcpy(nuevoCalculo.nombreJuegoMasBarato, puntero_string);
    printf("%s\n",nuevoCalculo.nombreJuegoMasBarato);
    
    puntero_string = strtok(NULL, "+");
    strcpy(nuevoCalculo.juegosGratis, puntero_string);
    printf("xd %s\n",nuevoCalculo.juegosGratis);
    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.precioJuegoMasCaro = atof(puntero_string);
    printf("%f\n",nuevoCalculo.precioJuegoMasCaro);
    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.precioJuegoMasBarato = atof(puntero_string);
    printf("%f\n",nuevoCalculo.precioJuegoMasBarato);
    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.sumaPreciosPorAnio = atof(puntero_string);
    printf("%f\n",nuevoCalculo.sumaPreciosPorAnio);
    //printf("%d %d %d %d %d %s %s %s %f %f %f", nuevoCalculo.anioJuego, nuevoCalculo.cantidadJuegos, nuevoCalculo.windows, nuevoCalculo.mac, nuevoCalculo.linuxx, nuevoCalculo.nombreJuegoMasCaro, nuevoCalculo.nombreJuegoMasBarato, nuevoCalculo.juegosGratis, nuevoCalculo.precioJuegoMasCaro, nuevoCalculo.precioJuegoMasBarato, nuevoCalculo.sumaPreciosPorAnio);
    return nuevoCalculo;
}
