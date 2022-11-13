#include "fworker.h"

int main(int argc, char *argv[])
{
    // LECTURA WORKERS
    char buff[2000];
    int contador = 0;
    char lectura[300000] = "";
    char *puntero_string;
    // precio minimo ingresado por flag
    int precioMinimo= atoi(argv[2]);
    // anio minimo ingresado por flag
    int anioMinimo= atoi(argv[1]);
    // Lectura de lineas enviadas por el broker
    while (1)
    {
        read(STDIN_FILENO, buff, 2000);
        if (strcmp(buff, "FIN") == 0)
        {
            break;
        }
        strcat(lectura, buff);
        strcat(lectura, "}");
        contador++;
    }

    // declaracion arreglo de lineas
    char **lineas = (char **)malloc(sizeof(char *) * (contador));
    // asignando memoria a el arreglo de lineas
    for (int i = 0; i < contador; i++)
    {
        lineas[i] = (char *)malloc(sizeof(char) * 500);
    }
    puntero_string = strtok(lectura, "}");
    // Se agregan las lineas a un arreglo de lineas
    for (int i = 0; i < contador; i++)
    {
        strcpy(lineas[i], puntero_string);
        puntero_string = strtok(NULL, "}");
    }
    // Se declara un arreglo para almacenar los juegos
    //juego *listadoJuegos = (juego *)malloc(sizeof(juego) * (contador));
    juego juegoAgregar;      
    int lenListadoJuegos=0;
    juego *listadoJuegos = (juego *)malloc(sizeof(juego) * (lenListadoJuegos));
    for (int i = 0; i < contador; i++)
    {
        juegoAgregar= lecturaJuego(lineas[i]);
        if((juegoAgregar.fechaLanzamiento >= anioMinimo) && (juegoAgregar.precio>=precioMinimo)){
            listadoJuegos=agregarJuegoLista(listadoJuegos,juegoAgregar,&lenListadoJuegos);
        }
    }


    // Liberando memoria de  los arreglos

    for (int i = 0; i < contador; i++)
    {
        free(lineas[i]);
    }
    free(lineas);

    // Empezando los calculos
    int cantidadJuegos = lenListadoJuegos;

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
    char juegosGratis[2500] = "-";
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
            strcpy(juegosGratis, "-");
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
        /* // Se ve que juegos están en la plataforma Windows.
        if (strcmp(listadoJuegos[i].windows, "Yes") == 0)
        {
            windows++;
        }
        // Se ve que juegos están en la plataforma Mac.
        if (strcmp(listadoJuegos[i].mac, "Yes") == 0)
        {
            mac++;
        }
        // Se ve que juegos están en la plataforma Linux.
        if (strcmp(listadoJuegos[i].linuxx, "Yes") == 0)
        {
            linuxx++;
        } */
        // Se ve que juegos están en la plataforma Windows.
        if (listadoJuegos[i].windows[0]=='Y')
        {
            windows++;
        }
        // Se ve que juegos están en la plataforma Mac.
        if (listadoJuegos[i].mac[0]=='Y')
        {
            mac++;
        }
        // Se ve que juegos están en la plataforma Linux.
        if (listadoJuegos[i].linuxx[0] == 'Y')
        {
            linuxx++;
        }
        // Se ve si el juego es gratis para almacenarlo en el string de juegos gratis
        if (strcmp(listadoJuegos[i].gratuidad, "True") == 0)
        {
            strcat(juegosGratis, listadoJuegos[i].nombre);
            strcat(juegosGratis, "\n");
        }
        // contador de juegos por anio
        cantidadJuegosPorAnio++;
        sumaPreciosPorAnio=listadoJuegos[i].precio+sumaPreciosPorAnio;
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

    // ENVIO DE CANTIDAD DE LINEAS LEIDAS POR EL WORKER
    if (write(STDOUT_FILENO, &cantidadJuegos, sizeof(cantidadJuegos)) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
    
    // ENVIO DE CANTIDAD DE AÑOS CALCULADOS POR EL WORKER
    if (write(STDOUT_FILENO, &cantidadAnios, sizeof(cantidadAnios)) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
    
    // Envio de los datos calculados por el worker
    for(int i=0; i<cantidadAnios;i++){
        if(write(STDOUT_FILENO,&listadoCalculosPorAnio[i],sizeof(calculosJuegoPorAnio)) == -1)
        {
            perror("Error al escribir en el archivo");
            exit(1);
        }
    }
    free(listadoJuegos);
    return 0;
}

