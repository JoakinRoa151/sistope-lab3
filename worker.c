#include "fworker.h"

int main(int argc, char *argv[])
{
    // LECTURA WORKERS
    char buff[1000];
    int contador = 0;
    char lectura[100000] = "";
    char *puntero_string;
    while (1)
    {
        read(STDIN_FILENO, buff, 1000);
        if (strcmp(buff, "FIN") == 0)
        {
            break;
        }
        strcat(lectura, buff);
        strcat(lectura, ",+,");
        contador++;
    }

    // declaracion arreglo de lineas
    char **lineas = (char **)malloc(sizeof(char *) * (contador));
    // asignando memoria a el arreglo de lineas
    for (int i = 0; i < contador; i++)
    {
        lineas[i] = (char *)malloc(sizeof(char) * 500);
    }
    puntero_string = strtok(lectura, "+");
    // Se agregan las lineas a
    for (int i = 0; i < contador; i++)
    {
        strcpy(lineas[i], puntero_string);
        puntero_string = strtok(NULL, "+");
    }

    juego *listadoJuegos = (juego *)malloc(sizeof(juego) * (contador));
    for (int i = 0; i < contador; i++)
    {
        listadoJuegos[i] = lecturaJuego(lineas[i]);
        // printf("ID:%d, nombre: %s, restriccion: %d, precio: %f, proximamente:%s, fechaLanzamiento: %d, gratuidad: %s, windows: %s,mac: %s,linux: %s\n", listadoJuegos[i].id, listadoJuegos[i].nombre, listadoJuegos[i].restriccion, listadoJuegos[i].precio, listadoJuegos[i].proximamente, listadoJuegos[i].fechaLanzamiento, listadoJuegos[i].gratuidad, listadoJuegos[i].windows, listadoJuegos[i].mac, listadoJuegos[i].linuxx);
    }

    // ESCRITURA WORKER
    // write(STDOUT_FILENO, "XDDDDDD", 7);}

    // Liberando memoria de  los arreglos

    for (int i = 0; i < contador; i++)
    {
        free(lineas[i]);
    }
    free(lineas);

    // Empezando los calculos
    int cantidadJuegos = contador;
    // Ordenamos la lista de juegos de el anio mas pequeño al mas grande

    mergeSort(listadoJuegos, 0, cantidadJuegos - 1);

    // Variables para calcular los datos necesarios
    char juegoMasCaro[100];
    strcpy(juegoMasCaro, listadoJuegos[0].nombre);
    char juegoMasBarato[100];
    strcpy(juegoMasBarato, listadoJuegos[0].nombre);
    float precioMasBarato = listadoJuegos[0].precio;
    float precioMasCaro = listadoJuegos[0].precio;
    calculosJuegoPorAnio calculoJuego;
    // Contadores necesarios para cada plataforma y cálculo total de precios.
    int windows = 0;
    int mac = 0;
    int linuxx = 0;
    int cantidadJuegosPorAnio = 0;
    int cantidadAnios = 0;
    float sumaPreciosPorAnio = 0;
    char juegosGratis[2500] = "";
    // Declaramos arreglo para guardar los datos de cada año
    int lenListadoCalculosPorAnio = 0;
    calculosJuegoPorAnio *listadoCalculosPorAnio = (calculosJuegoPorAnio *)malloc(sizeof(calculosJuegoPorAnio) * (lenListadoCalculosPorAnio));
    // anio a los que pertenecen los juegos que se esta calculando
    int anioJuegosActual = listadoJuegos[0].fechaLanzamiento;

    for (int i = 0; i < cantidadJuegos; i++)
    {
        if (anioJuegosActual != listadoJuegos[i].fechaLanzamiento)
        {
            // contador de anios
            cantidadAnios++;

            // Guardamos los datos calculados de un año
            calculoJuego.anioJuego = anioJuegosActual;
            calculoJuego.cantidadJuegos = cantidadJuegosPorAnio;
            strcpy(calculoJuego.nombreJuegoMasCaro, juegoMasCaro);
            strcpy(calculoJuego.nombreJuegoMasBarato, juegoMasBarato);
            strcpy(calculoJuego.juegosGratis, juegosGratis);
            calculoJuego.precioJuegoMasBarato = precioMasBarato;
            calculoJuego.precioJuegoMasCaro = precioMasCaro;
            calculoJuego.windows = windows;
            calculoJuego.mac = mac;
            calculoJuego.linuxx = linuxx;
            calculoJuego.sumaPreciosPorAnio = sumaPreciosPorAnio;

            // printf(" %d, %d, %d, %d, %s, %s, %lf, %lf, %lf \n ", calculoJuego.cantidadJuegos, calculoJuego.windows, calculoJuego.mac, calculoJuego.linuxx, calculoJuego.nombreJuegoMasCaro, calculoJuego.nombreJuegoMasBarato, calculoJuego.precioJuegoMasCaro, calculoJuego.precioJuegoMasBarato, calculoJuego.sumaPreciosPorAnio);
            // printf("%d %s\n", anioJuegosActual,calculoJuego.juegosGratis);
            // agregar este struct  a un array de structs
            listadoCalculosPorAnio = agregarCalculoLista(listadoCalculosPorAnio, calculoJuego, &lenListadoCalculosPorAnio);
            // Se hace un reset de las variables para calcular los datos de el siguiente año.
            anioJuegosActual = listadoJuegos[i].fechaLanzamiento;
            windows = 0;
            mac = 0;
            linuxx = 0;
            cantidadJuegosPorAnio = 0;
            sumaPreciosPorAnio = 0;
            strcpy(juegoMasCaro, listadoJuegos[i].nombre);
            strcpy(juegoMasBarato, listadoJuegos[i].nombre);
            strcpy(juegosGratis, "");
            precioMasBarato = listadoJuegos[i].precio;
            precioMasCaro = listadoJuegos[i].precio;
        }
        // Se calcula el juego mas caro.
        if (precioMasCaro < listadoJuegos[i].precio)
        {
            precioMasCaro = listadoJuegos[i].precio;
            strcpy(juegoMasCaro, listadoJuegos[i].nombre);
        }
        // Se calcula el juego mas barato
        if (precioMasBarato > listadoJuegos[i].precio)
        {
            precioMasBarato = listadoJuegos[i].precio;
            strcpy(juegoMasBarato, listadoJuegos[i].nombre);
        }
        // Se ve que juegos están en la plataforma Windows.
        if (listadoJuegos[i].windows[0]== 'Y')
        {
            windows++;
        }
        // Se ve que juegos están en la plataforma Mac.
        if (listadoJuegos[i].mac[0]=='Y')
        {
            mac++;
        }
        // Se ve que juegos están en la plataforma Linux.
        if (listadoJuegos[i].linuxx[0]=='Y')
        {
            linuxx++;
        }
        if (strcmp(listadoJuegos[i].gratuidad, "True") == 0)
        {
            strcat(juegosGratis, listadoJuegos[i].nombre);
            strcat(juegosGratis, "\n");
        }
        cantidadJuegosPorAnio++;
    }
    // Guardamos los datos calculados deL ultimo año
    calculoJuego.anioJuego = anioJuegosActual;
    calculoJuego.cantidadJuegos = cantidadJuegosPorAnio;
    strcpy(calculoJuego.nombreJuegoMasCaro, juegoMasCaro);
    strcpy(calculoJuego.nombreJuegoMasBarato, juegoMasBarato);
    calculoJuego.precioJuegoMasBarato = precioMasBarato;
    calculoJuego.precioJuegoMasCaro = precioMasCaro;
    calculoJuego.windows = windows;
    calculoJuego.mac = mac;
    calculoJuego.linuxx = linuxx;
    calculoJuego.sumaPreciosPorAnio = sumaPreciosPorAnio;
    strcpy(calculoJuego.juegosGratis, juegosGratis);
    cantidadAnios++;
    anioJuegosActual = listadoJuegos[cantidadAnios - 1].fechaLanzamiento;
    // agregar este struct  a un array de structs
    listadoCalculosPorAnio = agregarCalculoLista(listadoCalculosPorAnio, calculoJuego, &lenListadoCalculosPorAnio);
    // printf("cual es:%d %s\n",listadoCalculosPorAnio[0].cantidadJuegos, listadoCalculosPorAnio[0].juegosGratis);

    char aux[11000];
    // printf("contador: %d\n", contador);
    for (int i = 0; i < cantidadAnios; i++)
    {
        //printf("%d ,%d, %d, %d, %d, %s, %s, %f, %f, %f \n ", listadoCalculosPorAnio[i].anioJuego, listadoCalculosPorAnio[i].cantidadJuegos, listadoCalculosPorAnio[i].windows, listadoCalculosPorAnio[i].mac, listadoCalculosPorAnio[i].linuxx, listadoCalculosPorAnio[i].nombreJuegoMasCaro, listadoCalculosPorAnio[i].nombreJuegoMasBarato, listadoCalculosPorAnio[i].precioJuegoMasCaro, listadoCalculosPorAnio[i].precioJuegoMasBarato, listadoCalculosPorAnio[i].sumaPreciosPorAnio);
        //printf("Juegos gratis de este anio: %s \n", listadoCalculosPorAnio[i].juegosGratis);
        sprintf(aux, "%d+%d+%d+%d+%d+%s+%s+%s+%f+%f+%f+", listadoCalculosPorAnio[i].anioJuego, listadoCalculosPorAnio[i].cantidadJuegos, listadoCalculosPorAnio[i].windows, listadoCalculosPorAnio[i].mac, listadoCalculosPorAnio[i].linuxx, listadoCalculosPorAnio[i].nombreJuegoMasCaro, listadoCalculosPorAnio[i].nombreJuegoMasBarato, listadoCalculosPorAnio[i].juegosGratis, listadoCalculosPorAnio[i].precioJuegoMasCaro, listadoCalculosPorAnio[i].precioJuegoMasBarato, listadoCalculosPorAnio[i].sumaPreciosPorAnio);

        if (write(STDOUT_FILENO, aux, 11000) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
    }
    char buffer[11000] = "FIN";
    if (write(STDOUT_FILENO, buffer, 11000) == -1)
    {
        perror("Error al escribir en el archivo");
        exit(1);
    }

    free(listadoJuegos);
    free(listadoCalculosPorAnio);
    return 0;
}