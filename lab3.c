#include "funciones.h"

pthread_mutex_t lock;
FILE* archivoEntrada;
// Variables globales necesarias para el programa 
int anio = 0;
float precioMenor = 0;
int chunk= 1;
calculosJuegoPorAnio *listadoCalculosPorAnio;

int main(int argc, char* argv[]){
	extern char *optarg; //Almacena los argumentos introducidos después de haber indicado la variable.
    extern int optind; // Almacena el número de argumentos introducidos.
    int option;
    int iflag = 0;
    int oflag = 0;
    int error = 0;
    char nombre_archivo_salida[100];
    char nombre_archivo_entrada[100];
    int procesoPorPantalla = 0;
    int cantidadHebras = 1;
    while((option = getopt(argc, argv, "i:o:d:p:n:c:b")) != -1){
        switch(option){
            case 'i':
                iflag = 1;
                strcpy(nombre_archivo_entrada,optarg);
                break;
            case 'o':
                oflag = 1;
                strcpy(nombre_archivo_salida,optarg);
                break;
            case 'd':
                anio=atoi(optarg);
                break;
            case 'p':
                precioMenor=atof(optarg);
                break;
            case 'n':
                cantidadHebras=atoi(optarg);
                break;
            case 'c':
                chunk=atoi(optarg);
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
    if(iflag!=1){
        printf("Por favor ingresar nombre del archivo de entrada!!\n");
        return 0;
    }
    // Verificacion de archivo de salida
    if(oflag!=1){
        printf("Por favor ingresar nombre del archivo de salida!!\n");
        return 0;
    }
    // Verificacion de error
    if(error==1){
        printf("Por favor ingrese las variables de entrada!!\n");
        return 0;
    }
    // verificacion cantidad de workers
    if(cantidadHebras<1){
        printf("Por favor ingresar un numero de workers mayor a 0\n");
        return 0;
    }
    // Verificaciones del anio incial
    if(anio<0){
        printf("Por favor ingresar un numero de año mayor o igual a 0\n");
        return 0;
    }
    // Verificaciones el chunk
    if(chunk<0){
        printf("Por favor ingresar un chunk mayor o igual a 1\n");
        return 0;
    }
    // Verificaciones del precio menor.
    if(precioMenor<0){
        printf("Por favor ingresar un numero de precio mayor o igual a 0\n");
        return 0;
    }

    // Id de las hebras
    pthread_t hebrasId[cantidadHebras];
    listadoCalculosPorAnio = (calculosJuegoPorAnio *)malloc(sizeof(calculosJuegoPorAnio) * (0));

    // Se inicializa el mutex
    if(pthread_mutex_init(&lock, NULL) != 0){
        printf("Ha fallado el iniciar el mutex\n");
        return 1;
    }

    // Se abre el archivo
    archivoEntrada = fopen(nombre_archivo_entrada, "r");
    if(archivoEntrada == NULL){
        printf("No se pudo abrir el archivo\n");
        return 1;
    }
    
    

    // Se crean las hebras
    for(int i=0;i<cantidadHebras;i++){
        if(pthread_create(&(hebrasId[i]), NULL, &funcionHebra, NULL) != 0){
            printf("No se puedo crear la hebra\n");
            return 1;
        }
    }

    // Se esperan que terminen las hebras
    for(int i=0;i<cantidadHebras;i++){
        pthread_join(hebrasId[i], NULL);
    }

    // Se cierra el archivo de entrada cuando terminen las hebras
    fclose(archivoEntrada);

    // Se destruye el mutex
    pthread_mutex_destroy(&lock);
    
    return 0;
}
