#include "fworker.h"
// Descripción: Función que recibe un string con los datos de un juego y los separa en un struct juego.
// Entrada: String con los datos de un juego.
// Salida: Struct juego con los datos del juego.
juego lecturaJuego(char *lineaJuego) {
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
    puntero_string = strtok(NULL, "\n");
    strcpy(nuevoJuego.linuxx, puntero_string);
    // printf("ID:%d, nombre: %s, restriccion: %d, precio: %f, proximamente:%s, fechaLanzamiento: %d, gratuidad: %s, windows: %s,mac: %s,linux: %s\n", nuevoJuego.id, nuevoJuego.nombre, nuevoJuego.restriccion, nuevoJuego.precio, nuevoJuego.proximamente, nuevoJuego.fechaLanzamiento, nuevoJuego.gratuidad, nuevoJuego.windows, nuevoJuego.mac, nuevoJuego.linuxx);
    return nuevoJuego;
}

//Descripción: funcion que agrega datos de una lista tipo struct anio Posicion
//Entrada: lista de struct anioPosicion, posicion final, anio, tamaño de la lista
//Salida: lista de struct anioPosicion con un dato agregado
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

// Descripción: funcion que agrega datos de una lista tipo struct anio Posicion
// Entrada: lista de struct anioPosicion, posicion final, anio, tamaño de la lista
// Salida: lista de struct anioPosicion con un dato agregado
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

// Descripción: Función para calcular el promedio de los precios por año.
// Entrada: lista de juegos, tamaño de la lista.
// Salida: promedio de los precios.
float  calcularPromedioAnio(juego*listadoJuegosAnio, int n){
	float sumaPrecios=0;
	for(int i=0;i<n;i++){
		sumaPrecios=listadoJuegosAnio[i].precio+sumaPrecios;
	}
	return (sumaPrecios/n);
}


// Descripción: funcion que ordena el listado de juegos por fecha de lanzamiento
// Entrada: listado de juegos, inicio, medio, fin.
// Salida: Vacío.
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
Descripción: mergeSort que ordenara la lista de juegos
Entrada: Listado de juegos, inicio, fin
Salida: Vacío.
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
