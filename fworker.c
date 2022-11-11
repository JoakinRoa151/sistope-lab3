#include "fworker.h"

juego lecturaJuego(char *lineaJuego)
{
    char *puntero_string;
    juego nuevoJuego;
    puntero_string = strtok(lineaJuego, ",");
    nuevoJuego.id = atoi(puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoJuego.nombre, puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoJuego.restriccion = atoi(puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoJuego.precio = atof(puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoJuego.proximamente, puntero_string);
    puntero_string = strtok(NULL, ",");
    nuevoJuego.fechaLanzamiento = atoi(puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoJuego.gratuidad, puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoJuego.windows, puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoJuego.mac, puntero_string);
    puntero_string = strtok(NULL, ",");
    strcpy(nuevoJuego.linuxx, puntero_string);
    // printf("ID:%d, nombre: %s, restriccion: %d, precio: %f, proximamente:%s, fechaLanzamiento: %d, gratuidad: %s, windows: %s,mac: %s,linux: %s\n", nuevoJuego.id, nuevoJuego.nombre, nuevoJuego.restriccion, nuevoJuego.precio, nuevoJuego.proximamente, nuevoJuego.fechaLanzamiento, nuevoJuego.gratuidad, nuevoJuego.windows, nuevoJuego.mac, nuevoJuego.linuxx);
    return nuevoJuego;
}

/*
Entrada: lista de struct anioPosicion, posicion final, anio, tamaño de la lista
Salida: lista de struct anioPosicion con un dato agregado
Descripción: funcion que agrega datos de una lista tipo struct anio Posicion
*/
juego *agregarJuegoLista(juego *listaJuegos, juego juegoAgregar, int *n)
{
    juego *nuevaLista = malloc(sizeof(juego) * (*n + 1));
    for (int i = 0; i < *n; i++)
    {
        nuevaLista[i]=listaJuegos[i];
    }
    nuevaLista[*n]=juegoAgregar;
    *n = *n + 1;
    free(listaJuegos);
    return nuevaLista;
}

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
    //printf("\n %d \n\n",calculoAgregar.anioJuego);
    *n = *n + 1;
    free(listaCalculos);
    return nuevaLista;
}


//  ****************** FUNCIONES PARA CÁLCULOS ************************

// Función para calcular el juego mas caro.
int calcularJuegoMasCaro(juego *listadoJuegosAnio, int n)
{
    float maxPrecio = listadoJuegosAnio[0].precio;
    float maxTemporal;
    int posicionJuegoMasCaro;
    for (int i = 0; i < n; i++)
    {
        maxTemporal = listadoJuegosAnio[i].precio;
        if (maxTemporal > maxPrecio)
        {
            maxTemporal = listadoJuegosAnio[i].precio;
            posicionJuegoMasCaro = i;
        }
    }
    return posicionJuegoMasCaro;
}

// Función para calcular el juego mas barato.

juego menorPrecio(juego *listadoJuegosAnio, int n)
{
    float minPrecio = listadoJuegosAnio[0].precio;
    float auxiliarMinimo;
    juego juegoMasBarato;
    for (int i = 0; i < n; i++)
    {
        auxiliarMinimo = listadoJuegosAnio[i].precio;
        if (auxiliarMinimo < minPrecio)
        {
            auxiliarMinimo = listadoJuegosAnio[i].precio;
            juegoMasBarato = listadoJuegosAnio[i];
        }
    }
    return juegoMasBarato;
}

// Función para calcular el promedio de los precios por año

float  calcularPromedioAnio(juego*listadoJuegosAnio, int n){
	float sumaPrecios=0;
	for(int i=0;i<n;i++){
		sumaPrecios=listadoJuegosAnio[i].precio+sumaPrecios;
	}
	return (sumaPrecios/n);
}

// Función que obtiene el porcentaje de juegos que tiene cada plataforma por año

float * calcularPorcentajePlataforma(juego*listadoJuegosAnio, int n){
    float * porcentajesPlataforma=malloc(sizeof(float)*(3));
    float cantidadLin=0;
    float cantidadWin=0;
    float cantidadMac=0;
	for(int i=0;i<n;i++){
		if(strcmp(listadoJuegosAnio[i].windows,"Yes")){
            cantidadWin++;
        }
        if(strcmp(listadoJuegosAnio[i].linuxx,"Yes")){
            cantidadLin++;
        }
        if(strcmp(listadoJuegosAnio[i].mac,"Yes")){
            cantidadMac++;
        }
	}
	printf("cantidadWin: %f",cantidadWin);
    float total=cantidadWin+cantidadLin+cantidadMac;
	printf("total=%f \n\n",total);
    float porcentajeWin=(100 * cantidadLin)/total;
    float porcentajeLin=(100 * cantidadMac)/total;
    float porcentajeMac=(100 * cantidadWin)/total;
    porcentajesPlataforma[0]=porcentajeWin;
    porcentajesPlataforma[1]=porcentajeLin;
    porcentajesPlataforma[2]=porcentajeMac;
	return porcentajesPlataforma;
}
/*
Entrada: listado de juegos, inicio, medio, fin
Salida: -
Descripción: funcion que ordena el listado de juegos por fecha de lanzamiento
*/

void merge(juego* listadoJuegos,int inicio,int medio,int fin){
	
	juego* listaAux = (juego*)calloc(fin-inicio+1,sizeof(juego));
	int izq = inicio;
	int der = medio+1;
	int k = 0;

	
	while(izq<=medio && der<=fin){
		// Sentencia para que ordene por anio de lanzamiento
		if(listadoJuegos[izq].fechaLanzamiento<=listadoJuegos[der].fechaLanzamiento){
			
			listaAux[k] = listadoJuegos[izq];
			izq++;
		}else{
			
			listaAux[k] = listadoJuegos[der];
			der++;
		}
		k++;
	} 
	
	for(;izq<=medio;izq++){
		listaAux[k] = listadoJuegos[izq];
		k++;
	}

	for(;der<=fin;der++){
		listaAux[k] = listadoJuegos[der];
		k++;
	}

	int l=0;
	for(k=inicio;k<=fin;k++){
		listadoJuegos[k] = listaAux[l];
		l++;
	}
	free(listaAux);
	return;
}

/*
Entrada: Listado de juegos, inicio, fin
Salida: -
Descripción: mergeSort que ordenara la lista de juegos
*/
void mergeSort(juego* listadoJuegos, int inicio, int fin){

	if(inicio==fin){
		return;
	} else{ 
		int medio = (inicio+fin)/2;

		mergeSort(listadoJuegos,inicio,medio);

		mergeSort(listadoJuegos,medio+1,fin);

		merge(listadoJuegos,inicio,medio,fin);
		return;
	}
}



// Función que obtienes los juegos gratis de la plataforma.
/* int * obtenerJuegosGratis(int*listadoJuegosAnio, int n){
	int m=contadorJuegosGratis(listadoJuegosAnio,n);
    int * listadoJuegosGratis=malloc(sizeof(juego)*(m));
	for(int i=0;i<m;i++){
		 if(strcmp(listadoJuegosAnio[i].gratuidad,"True")){
            listadoJuegosGratis[i] =;
        }
	}

	return listadoJuegosGratis;
} */