#include "funciones.h"

// variables globales a utilizar
extern int chunk;
extern int anio;
extern float precioMenor;
extern FILE *archivoEntrada;
extern pthread_mutex_t lock;
extern int tamanioCalculos;
extern calculosJuegoPorAnio *listadoCalculosPorAnio;

// Descripción: funcion que ordena el listado de calculos por anio
// Entrada: listado de calculos, inicio, medio, fin.
// Salida: Vacío.
void merge(calculosJuegoPorAnio *calculos, int inicio, int medio, int fin)
{

    calculosJuegoPorAnio *listaAux = (calculosJuegoPorAnio *)calloc(fin - inicio + 1, sizeof(calculosJuegoPorAnio));
    int izq = inicio;
    int der = medio + 1;
    int k = 0;

    while (izq <= medio && der <= fin)
    {
        // Sentencia para que ordene por anio de lanzamiento
        if (calculos[izq].anioJuego <= calculos[der].anioJuego)
        {

            listaAux[k] = calculos[izq];
            izq++;
        }
        else
        {

            listaAux[k] = calculos[der];
            der++;
        }
        k++;
    }

    for (; izq <= medio; izq++)
    {
        listaAux[k] = calculos[izq];
        k++;
    }

    for (; der <= fin; der++)
    {
        listaAux[k] = calculos[der];
        k++;
    }

    int l = 0;
    for (k = inicio; k <= fin; k++)
    {
        calculos[k] = listaAux[l];
        l++;
    }
    free(listaAux);
    return;
}

/*
Descripción: mergeSort que ordenara la lista de calculo
Entrada: Listado de juegos, inicio, fin
Salida: Vacío.
*/
void mergeSort(calculosJuegoPorAnio *calculos, int inicio, int fin)
{

    if (inicio == fin)
    {
        return;
    }
    else
    {
        int medio = (inicio + fin) / 2;

        mergeSort(calculos, inicio, medio);

        mergeSort(calculos, medio + 1, fin);

        merge(calculos, inicio, medio, fin);
        return;
    }
}
// Descripción: funcion que verifica si existe un anio en una lista de calculos
// Entrada: lista de struct calculoJuegoPorAnio, juego de referencia,  tamaño de la lista
// Salida: booleano que indica si existe o no el anio
bool verificarAnio(calculosJuegoPorAnio *listadoCalculos, juego nuevoJuego, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (listadoCalculos[i].anioJuego == nuevoJuego.fechaLanzamiento)
        {
            return true;
        }
    }
    return false;
}

// Descripción: funcion que agrega datos de una lista tipo struct calculosJuegoPorAnio
// Entrada: lista de struct calculoJuegoPorAnio, calculo a agregar,  tamaño de la lista
// Salida: lista de struct calculoJuegoPorAnio con un calculo agregado
calculosJuegoPorAnio *agregarCalculoLista(calculosJuegoPorAnio *listaCalculos, calculosJuegoPorAnio calculoAgregar, int *n)
{
    calculosJuegoPorAnio *nuevaLista = malloc(sizeof(calculosJuegoPorAnio) * (*n + 1));
    for (int i = 0; i < *n; i++)
    {
        nuevaLista[i] = listaCalculos[i];
    }
    nuevaLista[*n] = calculoAgregar;
    // printf("\n %d \n\n",calculoAgregar.anioJuego);
    *n = *n + 1;
    free(listaCalculos);
    return nuevaLista;
}

// Descripción: funcion que agrega datos de una lista tipo struct anio Posicion
// Entrada: lista de struct anioPosicion, posicion final, anio, tamaño de la lista
// Salida: lista de struct anioPosicion con un dato agregado
calculosJuegoPorAnio calcularJuegoPorAnio(juego nuevoJuego)
{
    calculosJuegoPorAnio calculo;
    calculo.anioJuego = nuevoJuego.fechaLanzamiento;
    calculo.cantidadJuegos = 1;
    calculo.windows = 0;
    calculo.mac = 0;
    calculo.linuxx = 0;
    calculo.precioJuegoMasCaro = nuevoJuego.precio;
    calculo.precioJuegoMasBarato = nuevoJuego.precio;
    strcpy(calculo.nombreJuegoMasCaro, nuevoJuego.nombre);
    strcpy(calculo.nombreJuegoMasBarato, nuevoJuego.nombre);
    strcpy(calculo.juegosGratis, "-");
    if (nuevoJuego.gratuidad[0] == 'T')
    {
        strcpy(calculo.juegosGratis, nuevoJuego.nombre);
    }
    // Se ve que juegos están en la plataforma Windows.
    if (nuevoJuego.windows[0] == 'Y')
    {
        calculo.windows = 1;
    }
    // Se ve que juegos están en la plataforma Mac.
    if (nuevoJuego.mac[0] == 'Y')
    {
        calculo.mac = 1;
    }
    // Se ve que juegos están en la plataforma Linux.
    if (nuevoJuego.linuxx[0] == 'Y')
    {
        calculo.linuxx = 1;
    }
    calculo.sumaPreciosPorAnio = nuevoJuego.precio;

    // printf("\n %d %s \n\n", calculo.anioJuego, calculo.nombreJuegoMasCaro);

    return calculo;
}
// Descripción: Función que busca la posicion de calculo en la lista de calculos de un año en especifico
// Entrada: lista de calculos de anio, nuevo juego, tamaño de la listas
// Salida: int con la posicion
int buscarPosicionCalculo(calculosJuegoPorAnio *listaCalculos, int anio, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (listaCalculos[i].anioJuego == anio)
        {
            return i;
        }
    }
    return 0;
}

// Descripción: Función que compara los calculo de anio con un nuevo juego de ese mismo anio
// Entrada: lista de calculos de anio, nuevo juego, tamanio del arreglo
// Salida: -
void compararCalculos(calculosJuegoPorAnio *listadoCalculosPorAnio, juego nuevoJuego, int tamanioCalculos)
{
    int posicionCalculo = buscarPosicionCalculo(listadoCalculosPorAnio, nuevoJuego.fechaLanzamiento, tamanioCalculos);
    // Se calcula el juego mas caro.
    if (listadoCalculosPorAnio[posicionCalculo].precioJuegoMasCaro < nuevoJuego.precio)
    {
        listadoCalculosPorAnio[posicionCalculo].precioJuegoMasCaro = nuevoJuego.precio;
        strcpy(listadoCalculosPorAnio[posicionCalculo].nombreJuegoMasCaro, nuevoJuego.nombre);
    }
    // Se calcula el juego mas barato
    if (listadoCalculosPorAnio[posicionCalculo].precioJuegoMasBarato > nuevoJuego.precio)
    {
        listadoCalculosPorAnio[posicionCalculo].precioJuegoMasBarato = nuevoJuego.precio;
        strcpy(listadoCalculosPorAnio[posicionCalculo].nombreJuegoMasBarato, nuevoJuego.nombre);
    }
    // Se ve que juegos están en la plataforma Windows.
    if (nuevoJuego.windows[0] == 'Y')
    {
        listadoCalculosPorAnio[posicionCalculo].windows++;
    }
    // Se ve que juegos están en la plataforma Mac.
    if (nuevoJuego.mac[0] == 'Y')
    {
        listadoCalculosPorAnio[posicionCalculo].mac++;
    }
    // Se ve que juegos están en la plataforma Linux.
    if (nuevoJuego.linuxx[0] == 'Y')
    {
        listadoCalculosPorAnio[posicionCalculo].linuxx++;
    }
    // Se ve si el juego es gratis para almacenarlo en el string de juegos gratis
    if (nuevoJuego.gratuidad[0] == 'T')
    {
        strcat(listadoCalculosPorAnio[posicionCalculo].juegosGratis, nuevoJuego.nombre);
        strcat(listadoCalculosPorAnio[posicionCalculo].juegosGratis, "\n");
    }
    listadoCalculosPorAnio[posicionCalculo].sumaPreciosPorAnio = listadoCalculosPorAnio[posicionCalculo].sumaPreciosPorAnio + nuevoJuego.precio;
    listadoCalculosPorAnio[posicionCalculo].cantidadJuegos++;
}

// Descripción: Función que recibe un string con los datos de un juego y los separa en un struct juego.
// Entrada: String con los datos de un juego.
// Salida: Struct juego con los datos del juego.
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
    puntero_string = strtok(NULL, "\n");
    strcpy(nuevoJuego.linuxx, puntero_string);
    // printf("ID:%d, nombre: %s, restriccion: %d, precio: %f, proximamente:%s, fechaLanzamiento: %d, gratuidad: %s, windows: %s,mac: %s,linux: %s\n", nuevoJuego.id, nuevoJuego.nombre, nuevoJuego.restriccion, nuevoJuego.precio, nuevoJuego.proximamente, nuevoJuego.fechaLanzamiento, nuevoJuego.gratuidad, nuevoJuego.windows, nuevoJuego.mac, nuevoJuego.linuxx);
    return nuevoJuego;
}

// Descripción: Función que
// Entrada: array.
// Salida: void.
void *funcionHebra(void *arg)
{
    // ciclo antes de que se termine el archivo
    while (!feof(archivoEntrada))
    {
        char buffer[2000];
        // seccion critica
        pthread_mutex_lock(&lock);
        // for con el numero de lineas que se van a leer
        for (int i = 0; i < chunk; i++)
        {
            // en caso de que se termine el archivo se sale del for
            if (fgets(buffer, 2000, archivoEntrada) == NULL)
            {
                break;
            }
            // se lee el juego
            juego nuevoJuego = lecturaJuego(buffer);
            // se calculan los datos del juego
            if ((precioMenor < nuevoJuego.precio || nuevoJuego.gratuidad[0]=='T') && anio <= nuevoJuego.fechaLanzamiento)
            {
                // si es que ya existe el calculo de ese año se actualiza comparandolo con el nuevo juego
                if (verificarAnio(listadoCalculosPorAnio, nuevoJuego, tamanioCalculos))
                {
                    compararCalculos(listadoCalculosPorAnio, nuevoJuego, tamanioCalculos);
                }
                else
                {
                    // si no existe el calculo de ese año se crea uno nuevo
                    calculosJuegoPorAnio nuevoCalculo = calcularJuegoPorAnio(nuevoJuego);
                    listadoCalculosPorAnio = agregarCalculoLista(listadoCalculosPorAnio, nuevoCalculo, &tamanioCalculos);
                }
            }
        }
        // se sale de la seccion critica
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}