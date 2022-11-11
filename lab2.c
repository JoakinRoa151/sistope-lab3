#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){
	extern char *optarg; //Almacena los argumentos introducidos después de haber indicado la variable.
    extern int optind; // Almacena el número de argumentos introducidos.
    int option;
    int iflag = 0;
    int oflag = 0;
    int error = 0;
	char anio[100] = "0";
	char precioMenor[100] = "0";
    char nombre_archivo_salida[100];
    char nombre_archivo_entrada[100];
    char procesoPorPantalla[100] = "0";
    char cantidadWorkers[100] = "1";
    while((option = getopt(argc, argv, "i:o:d:p:n:b")) != -1){
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
                strcpy(anio,optarg);
                break;
            case 'p':
                strcpy(precioMenor,optarg);
                break;
            case 'n':
                strcpy(cantidadWorkers,optarg);
                break;
            case 'b':
                strcpy(procesoPorPantalla,"1");
                break;
            case '?':
                error = 1;
                break;
            exit(0);
        }
    }
    if(iflag!=1){
        printf("porfavor ingresar nombre del archivo de entrada!!\n");
        return 0;
    }
    if(oflag!=1){
        printf("porfavor ingresar nombre del archivo de salida!!\n");
        return 0;
    }
    if(error==1){
        printf("porfavor ingrese las variables de entrada!!\n");
        return 0;
    }
    // printf("anio: %s\n",anio);
	// printf("precio menor: %s\n",precioMenor);
	// printf("Nombre archivo salida: %s\n",nombre_archivo_salida);
	// printf("Nmbre archivo entrada: %s\n",nombre_archivo_entrada);
	// printf("workers: %s\n",cantidadWorkers);
    // printf("bflag: %s\n",procesoPorPantalla);
    
        int pid= fork();
        if(pid==0){
            execlp("./broker", "broker", nombre_archivo_entrada,nombre_archivo_salida, anio, precioMenor, cantidadWorkers,procesoPorPantalla,NULL);
        }
    return 0;
}
