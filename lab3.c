#include "funciones.h"

pthread_mutex_t lock;
FILE *archivoEntrada;
// Variables globales necesarias para el programa
int anio = 0;
float precioMenor = 0;
int chunk = 1;
calculosJuegoPorAnio *listadoCalculosPorAnio;
int tamanioCalculos = 0;

int main(int argc, char *argv[])
{
    extern char *optarg; // Almacena los argumentos introducidos después de haber indicado la variable.
    extern int optind;   // Almacena el número de argumentos introducidos.
    int option;
    int iflag = 0;
    int oflag = 0;
    int error = 0;
    char nombre_archivo_salida[100];
    char nombre_archivo_entrada[100];
    int procesoPorPantalla = 0;
    int cantidadHebras = 1;
    while ((option = getopt(argc, argv, "i:o:d:p:n:c:b")) != -1)
    {
        switch (option)
        {
        case 'i':
            iflag = 1;
            strcpy(nombre_archivo_entrada, optarg);
            break;
        case 'o':
            oflag = 1;
            strcpy(nombre_archivo_salida, optarg);
            break;
        case 'd':
            anio = atoi(optarg);
            break;
        case 'p':
            precioMenor = atof(optarg);
            break;
        case 'n':
            cantidadHebras = atoi(optarg);
            break;
        case 'c':
            chunk = atoi(optarg);
            break;
        case 'b':
            procesoPorPantalla = 1;
            break;
        case '?':
            error = 1;
            break;
            exit(0);
        }
    }
    // Verificaciones de archivo de entradas
    if (iflag != 1)
    {
        printf("Por favor ingresar nombre del archivo de entrada!!\n");
        return 0;
    }
    // Verificacion de archivo de salida
    if (oflag != 1)
    {
        printf("Por favor ingresar nombre del archivo de salida!!\n");
        return 0;
    }
    // Verificacion de error
    if (error == 1)
    {
        printf("Por favor ingrese las variables de entrada!!\n");
        return 0;
    }
    // verificacion cantidad de workers
    if (cantidadHebras < 1)
    {
        printf("Por favor ingresar un numero de workers mayor a 0\n");
        return 0;
    }
    // Verificaciones del anio incial
    if (anio < 0)
    {
        printf("Por favor ingresar un numero de año mayor o igual a 0\n");
        return 0;
    }
    // Verificaciones el chunk
    if (chunk < 0)
    {
        printf("Por favor ingresar un chunk mayor o igual a 1\n");
        return 0;
    }
    // Verificaciones del precio menor.
    if (precioMenor < 0)
    {
        printf("Por favor ingresar un numero de precio mayor o igual a 0\n");
        return 0;
    }

    // Id de las hebras
    pthread_t hebrasId[cantidadHebras];
    listadoCalculosPorAnio = (calculosJuegoPorAnio *)malloc(sizeof(calculosJuegoPorAnio) * (tamanioCalculos));

    // Se inicializa el mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Ha fallado el iniciar el mutex\n");
        return 1;
    }

    // Se abre el archivo
    archivoEntrada = fopen(nombre_archivo_entrada, "r");
    if (archivoEntrada == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }

    // Se crean las hebras
    for (int i = 0; i < cantidadHebras; i++)
    {
        if (pthread_create(&(hebrasId[i]), NULL, &funcionHebra, NULL) != 0)
        {
            printf("No se puedo crear la hebra\n");
            return 1;
        }
    }

    // Se esperan que terminen las hebras
    for (int i = 0; i < cantidadHebras; i++)
    {
        pthread_join(hebrasId[i], NULL);
    }

    // Se cierra el archivo de entrada cuando terminen las hebras
    fclose(archivoEntrada);

    if (tamanioCalculos == 0)
    {
        // Se destruye el mutex
        pthread_mutex_destroy(&lock);
        printf("No se encontraron juegos con el año ingresado o precio minimo ingresado\n");
        exit(0);
    }
    // se ordena el listado de calculos por anio de menor a mayor
    mergeSort(listadoCalculosPorAnio, 0, tamanioCalculos - 1);

    // ESCRITURA DEL ARCHIVO FINAL
    FILE *archivoSalida = fopen(nombre_archivo_salida, "w");
    for (int i = 0; i < tamanioCalculos; i++)
    {
        // caso de que exista un precio menor muy grande
        if (listadoCalculosPorAnio[i].precioJuegoMasCaro < precioMenor)
        {
            fprintf(archivoSalida, "ANIO: %d \n", listadoCalculosPorAnio[i].anioJuego);
            fprintf(archivoSalida, "No se han encontrado Juegos con un precio mayor: %d \n", listadoCalculosPorAnio[i].anioJuego);
            fprintf(archivoSalida, "Juegos gratis:\n %s \n\n", listadoCalculosPorAnio[i].juegosGratis);
        }
        else
        {
            float porcentaje_win, porcentaje_mac, porcentaje_linuxx, promedioPrecios;
            porcentaje_win = ((float)listadoCalculosPorAnio[i].windows / (float)listadoCalculosPorAnio[i].cantidadJuegos) * 100;
            porcentaje_mac = ((float)listadoCalculosPorAnio[i].mac / (float)listadoCalculosPorAnio[i].cantidadJuegos) * 100;
            porcentaje_linuxx = ((float)listadoCalculosPorAnio[i].linuxx / (float)listadoCalculosPorAnio[i].cantidadJuegos) * 100;
            promedioPrecios = listadoCalculosPorAnio[i].sumaPreciosPorAnio / listadoCalculosPorAnio[i].cantidadJuegos;
            fprintf(archivoSalida, "ANIO: %d \n", listadoCalculosPorAnio[i].anioJuego);
            fprintf(archivoSalida, "Juego mas Caro: %s %f\n", listadoCalculosPorAnio[i].nombreJuegoMasCaro, listadoCalculosPorAnio[i].precioJuegoMasCaro);
            fprintf(archivoSalida, "Juego mas Barato: %s %f \n", listadoCalculosPorAnio[i].nombreJuegoMasBarato, listadoCalculosPorAnio[i].precioJuegoMasBarato);
            fprintf(archivoSalida, "Promedio de precios: %f \n", promedioPrecios);
            fprintf(archivoSalida, "Windows: %f Mac: %f Linux: %f \n", porcentaje_win, porcentaje_mac, porcentaje_linuxx);
            fprintf(archivoSalida, "Juegos gratis:\n %s \n\n", listadoCalculosPorAnio[i].juegosGratis);
        }
    }
    fclose(archivoSalida);
    // IMPRESION POR PANTALLA EN CASO DE QUE EXISTA LA FLAG
    if (procesoPorPantalla == 1)
    {
        for (int i = 0; i < tamanioCalculos; i++)
        {
            if (listadoCalculosPorAnio[i].precioJuegoMasCaro < precioMenor)
            {
                printf("ANIO: %d \n", listadoCalculosPorAnio[i].anioJuego);
                printf("No se han encontrado Juegos con un precio mayor: %d \n", listadoCalculosPorAnio[i].anioJuego);
                printf("Juegos gratis:\n %s \n\n", listadoCalculosPorAnio[i].juegosGratis);
            }
            else
            {
                float porcentaje_win, porcentaje_mac, porcentaje_linuxx, promedioPrecios;
                porcentaje_win = ((float)listadoCalculosPorAnio[i].windows / (float)listadoCalculosPorAnio[i].cantidadJuegos) * 100;
                porcentaje_mac = ((float)listadoCalculosPorAnio[i].mac / (float)listadoCalculosPorAnio[i].cantidadJuegos) * 100;
                porcentaje_linuxx = ((float)listadoCalculosPorAnio[i].linuxx / (float)listadoCalculosPorAnio[i].cantidadJuegos) * 100;
                promedioPrecios = listadoCalculosPorAnio[i].sumaPreciosPorAnio / listadoCalculosPorAnio[i].cantidadJuegos;
                printf("ANIO: %d \n", listadoCalculosPorAnio[i].anioJuego);
                printf("Juego mas Caro: %s %f \n", listadoCalculosPorAnio[i].nombreJuegoMasCaro, listadoCalculosPorAnio[i].precioJuegoMasCaro);
                printf("Juego mas Barato: %s %f \n", listadoCalculosPorAnio[i].nombreJuegoMasBarato, listadoCalculosPorAnio[i].precioJuegoMasBarato);
                printf("Promedio de precios: %f \n", promedioPrecios);
                printf("Windows: %f Mac: %f Linux: %f \n", porcentaje_win, porcentaje_mac, porcentaje_linuxx);
                printf("Juegos gratis:\n %s \n\n", listadoCalculosPorAnio[i].juegosGratis);
            }
        }
    }

    // Se destruye el mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
