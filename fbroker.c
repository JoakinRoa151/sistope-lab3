#include "fbroker.h"

/*
Descripción: funcion que agrega datos de una lista tipo struct calculoJuegoPorAnio
Entrada: lista de struct calculosJuegoPorAnio , calculo que se agregara, tamaño de la lista
Salida: lista de struct calculosJuegoPorAnio  con un dato agregado
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

int verificarCalculoEnLista(calculosJuegoPorAnio * listaCalculos,calculosJuegoPorAnio calculo, int n){
    for(int i=0;i<n;i++){
        if(listaCalculos[i].anioJuego == calculo.anioJuego){
            return 1;
        }
    }
    return 0;
}
// Descripcion: funcion que agrega un elemento a una lista
// Entrada: lista, elemento, tamaño de la lista
// Salida: retornar lista
int* append(int* lista, int elemento, int *n){
    //printf("cantElem: %d",*n);
    int* new=(int*)malloc((*n+1)*sizeof(int));
    for (int i=0;i<*n;i++){
        new[i]=lista[i];
    }
    new[*n]=elemento;
    (*n)++;
    //free(lista);
    return new;
}

// Descripción: Función que revisa si está en la lista.
// Entrada: Lista de enteros, elemento a buscar, cantidad.
// Salida: 1 si está, 0 si no está.
int inList(int elem,int *lista, int cant){
    for(int i=0; i<cant; i++){
        if(lista[i]==elem){
            return 1;
        }
    }
    return 0;
}
// Descripción: funcion que lee un archivo de texto y lo guarda en una lista con la cantidad de años.
// Entrada: nombre del archivo de texto, lista de struct anioPosicion, posicion final, anio, tamaño de la lista
// Salida: lista de struct anioPosicion con un dato agregado
int* aniosEnLista(calculosJuegoPorAnio ** lista, int n, int *tamanio, int *elem){
    int* listaAnios;
    for(int i=0;i<n;i++){
        for(int j=0;j<tamanio[i];j++){          
            if(!inList(lista[i][j].anioJuego,listaAnios, (*elem))){
                listaAnios= append(listaAnios,lista[i][j].anioJuego, &(*elem));
            }
        }
    }
    return listaAnios;
}


// Descripción: Función que traspasa de un string a struct.
// Entrada: array.
// Salida: array con los juegos por año.
calculosJuegoPorAnio lecturaCalculo(char *lineaCalculo)
{
    char *puntero_string;
    
    calculosJuegoPorAnio nuevoCalculo;
    puntero_string = strtok(lineaCalculo, "+");
    nuevoCalculo.anioJuego = atoi(puntero_string);
    // Secuencia de llamadas a strtok para colocar "+" como separador.
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.cantidadJuegos = atoi(puntero_string);
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.windows = atoi(puntero_string);
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.mac = atoi(puntero_string);    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.linuxx = atoi(puntero_string);   
    puntero_string = strtok(NULL, "+");
    strcpy(nuevoCalculo.nombreJuegoMasCaro, puntero_string);    
    puntero_string = strtok(NULL, "+");
    strcpy(nuevoCalculo.nombreJuegoMasBarato, puntero_string);
    puntero_string = strtok(NULL, "+");
    strcpy(nuevoCalculo.juegosGratis, puntero_string);
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.precioJuegoMasCaro = atof(puntero_string);
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.precioJuegoMasBarato = atof(puntero_string);    
    puntero_string = strtok(NULL, "+");
    nuevoCalculo.sumaPreciosPorAnio = atof(puntero_string);
    return nuevoCalculo;
}

// Descripcion: funcion que lee el archivo de calculos y lo guarda en una lista.
// Entrada: nombre del archivo, lista de calculos, tamaño de la lista.
// Salida: lista de calculos.
calculosJuegoPorAnio * evaluacionCalculo(calculosJuegoPorAnio * listaCalculo, calculosJuegoPorAnio calculoPorAnio, int * banderas, int * listaAnios, int cantidadAniosTotales){
    for(int i=0;i<cantidadAniosTotales;i++){
        if(calculoPorAnio.anioJuego == listaAnios[i] && banderas[i] == 0){
            listaCalculo[i] = calculoPorAnio;
            banderas[i] = 1;
            return listaCalculo;
        }
        if(calculoPorAnio.anioJuego == listaAnios[i] && banderas[i] == 1){
            if (listaCalculo[i].precioJuegoMasCaro < calculoPorAnio.precioJuegoMasCaro)
            {
                listaCalculo[i].precioJuegoMasCaro= calculoPorAnio.precioJuegoMasCaro;
                strcpy(listaCalculo[i].nombreJuegoMasCaro , calculoPorAnio.nombreJuegoMasCaro);
            }
            // Se calcula el juego mas barato
            if (listaCalculo[i].precioJuegoMasBarato > calculoPorAnio.precioJuegoMasBarato)
            {
                listaCalculo[i].precioJuegoMasBarato= calculoPorAnio.precioJuegoMasBarato;
                strcpy(listaCalculo[i].nombreJuegoMasBarato , calculoPorAnio.nombreJuegoMasBarato);
            }
            if (strlen(calculoPorAnio.juegosGratis) > 0)
            {
                strcat(listaCalculo[i].juegosGratis, calculoPorAnio.juegosGratis);
            }
            listaCalculo[i].windows=listaCalculo[i].windows+calculoPorAnio.windows;
            listaCalculo[i].mac =listaCalculo[i].mac+calculoPorAnio.mac;
            listaCalculo[i].linuxx= listaCalculo[i].linuxx+calculoPorAnio.linuxx;
            listaCalculo[i].sumaPreciosPorAnio=listaCalculo[i].sumaPreciosPorAnio+calculoPorAnio.sumaPreciosPorAnio;
            listaCalculo[i].cantidadJuegos=listaCalculo[i].cantidadJuegos+calculoPorAnio.cantidadJuegos;
            return listaCalculo;
        }
    }
    return listaCalculo;
}


// Descripción: funcion que ordena el listado de juegos por fecha de lanzamiento
// Entrada: listado de juegos, inicio, medio, fin.
// Salida: Vacío.
void merge(calculosJuegoPorAnio* calculos,int inicio,int medio,int fin){
	
	calculosJuegoPorAnio* listaAux = (calculosJuegoPorAnio*)calloc(fin-inicio+1,sizeof(calculosJuegoPorAnio));
	int izq = inicio;
	int der = medio+1;
	int k = 0;

	
	while(izq<=medio && der<=fin){
		// Sentencia para que ordene por anio de lanzamiento
		if(calculos[izq].anioJuego<=calculos[der].anioJuego){
			
			listaAux[k] = calculos[izq];
			izq++;
		}else{
			
			listaAux[k] = calculos[der];
			der++;
		}
		k++;
	} 
	
	for(;izq<=medio;izq++){
		listaAux[k] = calculos[izq];
		k++;
	}

	for(;der<=fin;der++){
		listaAux[k] = calculos[der];
		k++;
	}

	int l=0;
	for(k=inicio;k<=fin;k++){
		calculos[k] = listaAux[l];
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
void mergeSort(calculosJuegoPorAnio* calculos, int inicio, int fin){

	if(inicio==fin){
		return;
	} else{ 
		int medio = (inicio+fin)/2;

		mergeSort(calculos,inicio,medio);

		mergeSort(calculos,medio+1,fin);

		merge(calculos,inicio,medio,fin);
		return;
	}
}



