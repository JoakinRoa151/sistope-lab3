#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include "padre.h"
#include "hijo.h"
#include "estructura.h"

int main(int argc, char* argv[]){
	extern char *optarg; //Almacena los argumentos introducidos después de haber indicado la variable.
    extern int optind; // Almacena el número de argumentos introducidos.
    int option;
    int iflag = 0;
    int oflag = 0;
    int dflag = 0;
    int pflag = 0;
    int bflag = 0;
    int error = 0;
	int anio;
	int precioMenor;
    char * nombre_archivo_salida;
    char * nombre_archivo_entrada;
    while((option = getopt(argc, argv, "i:o:d:p:b")) != -1){
        switch(option){
            case 'i':
                iflag = 1;
                nombre_archivo_entrada = optarg;
                break;
            case 'o':
                oflag = 1;
                nombre_archivo_salida = optarg;
                break;
            case 'd':
                dflag = 1;
                anio = atoi(optarg);
                break;
            case 'p':
                pflag = 1;
                precioMenor = atoi(optarg);
                break;
            case 'b':
                bflag = 1;
                break;
            case '?':
                error = 1;
                break;
            exit(0);
        }
    }
	printf("Esta es i: %d\n",iflag);
	printf("Esta es o: %d\n",oflag);
	printf("Esta es d: %d\n",dflag);
	printf("Esta es p: %d\n",pflag);
	printf("Esta es b: %d\n",bflag);
	printf("Esta es error: %d\n",error);
	printf("Este es el año solicitado: %d", anio);
	printf("Este es el precio menor: %d", precioMenor);
	printf("Este es el nombre del archivo de salida: %s", nombre_archivo_salida);
    // Se ingresa por comando el nombre del archivo que se debe leer
    FILE * archivo = fopen(nombre_archivo_entrada, "r");
	int contador=0;
    char buffer[100000];
	char * puntero_string;
	// si el archivo no existe
	if(archivo == NULL){
		printf("Error: el archivo de entrada no existe.");
	}
	// se obtiene el numero de lineas del archivo 
	// que es igual a la cantidad de juegos que debe leer 
	int numJuegosTotal=contarLineas(nombre_archivo_entrada);
	// se crea un arreglo de estructuras de tipo juego
	juego * listadoJuegos =(juego *)malloc(sizeof(juego)*(numJuegosTotal));
	// se lee el archivo linea por linea y se agrega al arreglo de juegos
	printf("\n primera lectura del archivo de entrada \n");
	while(fgets(buffer,numJuegosTotal,archivo) != NULL){
		puntero_string = strtok(buffer,",");
		listadoJuegos[contador].id = atoi(puntero_string);
		puntero_string = strtok(NULL,",");
		strcpy(listadoJuegos[contador].nombre,puntero_string);
		puntero_string = strtok(NULL,",");
		listadoJuegos[contador].restriccion= atoi(puntero_string);
		puntero_string = strtok(NULL,",");
		listadoJuegos[contador].precio = atof(puntero_string);
		puntero_string = strtok(NULL,",");
		strcpy(listadoJuegos[contador].proximamente,puntero_string);
		puntero_string = strtok(NULL,",");
		listadoJuegos[contador].fechaLanzamiento = atoi(puntero_string);
		puntero_string = strtok(NULL,",");
		strcpy(listadoJuegos[contador].gratuidad, puntero_string);
		puntero_string = strtok(NULL,",");
		strcpy(listadoJuegos[contador].windows,puntero_string);
		puntero_string = strtok(NULL,",");
		strcpy(listadoJuegos[contador].mac,puntero_string);
		puntero_string = strtok(NULL,",");
		strcpy(listadoJuegos[contador].linuxx,puntero_string);
		printf("ID:%d, nombre: %s, restriccion: %d, precio: %f, proximamente:%s, fechaLanzamiento: %d, gratuidad: %s, windows: %s,mac: %s,linux: %s\n",listadoJuegos[contador].id,listadoJuegos[contador].nombre,listadoJuegos[contador].restriccion,listadoJuegos[contador].precio,listadoJuegos[contador].proximamente,listadoJuegos[contador].fechaLanzamiento,listadoJuegos[contador].gratuidad,listadoJuegos[contador].windows,listadoJuegos[contador].mac,listadoJuegos[contador].linuxx);
		contador++;
	}
	// se ordena el arreglo por fecha de lanzamiento
	mergeSort(listadoJuegos,0,numJuegosTotal-1);
	int numeroAnios=0;
	// se escribe el archivo intermedio con los juegos ordenados por fecha de lanzamiento
	anioPosicion * posicionesFinales=escribirListadoJuego("salidaOrdenada.txt",listadoJuegos,numJuegosTotal,&numeroAnios);
	
	
	fclose(archivo);

	// variables para el manejo de los procesos
	int posicionInicioAnio=0;
	char buff[1000];
	char buff2[1000];

	// pid de los procesos hijos;
	int hijos_pid[numeroAnios];

	// pipes
	int fds1[numeroAnios][2];
	int fds2[numeroAnios][2];
	//cantidad de pipes por proceso que es igual al numero de anios
	
	for(int cont = 0; cont < numeroAnios; cont++){
		pipe(fds1[cont]);
		pipe(fds2[cont]);
	}
	// se crea un proceso hijo por cada anios
	for (int contador = 0; contador<numeroAnios; contador++) {
			hijos_pid[contador] = fork();
			// en caso de error
			if(hijos_pid[contador] == -1){
				printf("Error al crear el proceso hijo");
				exit(1);
			}else if(hijos_pid[contador] == 0){
				// proceso hijo
				// cerramos los pipes que no vamos a usar
				close(fds1[contador][1]);
				close(fds2[contador][0]);

				// el hijo lee del pipe 1
            	if(read(fds1[contador][0],buff2,sizeof(buff2)) == -1){
					printf("Error al leer del pipe");
					exit(1);
				}

				// posicion inicio anio
				puntero_string = strtok(buff2,",");
				int posicionInicialJuego=atoi(puntero_string);

				// posicion final anio
				puntero_string = strtok(NULL,",");
				int posicionFinalJuego=atoi(puntero_string);

				// anio
				puntero_string = strtok(NULL,",");
				int anioLanzamientoJuego=atoi(puntero_string);

				//printf("Posicion inicial  %d POSICION FINAL %d para el hijo del ANIO %d",posicionInicialJuego,posicionFinalJuego,anioLanzamientoJuego);
				// calculos del hijo
				int cantidadJuegosPorAnio=contarLineasPorAnio("salidaOrdenada.txt",posicionInicialJuego,posicionFinalJuego);
				juego * listadoJuegosPorAnio=lecturaHijo(posicionInicialJuego,posicionFinalJuego,"salidaOrdenada.txt");
				
				int posCaro = calcularJuegoMasCaro(listadoJuegosPorAnio,cantidadJuegosPorAnio );
				char * juegoMasCaroNombre = listadoJuegosPorAnio[posCaro].nombre;
				printf("El juego mas caro del anio %d es %s\n",anioLanzamientoJuego,juegoMasCaroNombre);
				float * listadosPorcentajePlataforma=calcularPorcentajePlataforma(listadoJuegosPorAnio,cantidadJuegosPorAnio);
				printf("El porcentaje de juegos para windows es %f\n",listadosPorcentajePlataforma[0]);
				printf("El porcentaje de juegos para mac es %f\n",listadosPorcentajePlataforma[1]);
				printf("El porcentaje de juegos para linux es %f\n",listadosPorcentajePlataforma[2]);
				float porcentajeWin=listadosPorcentajePlataforma[0];
				float porcentajeMac=listadosPorcentajePlataforma[1];
				float porcentajeLinux=listadosPorcentajePlataforma[2];
				// cada hijo escribe en el archivo de salida para mostrar algun avance del laboratorio
				escrituraArchivoHijo(nombre_archivo_salida,anioLanzamientoJuego,juegoMasCaroNombre,porcentajeWin,porcentajeMac,porcentajeLinux);

				// se deberia escribir en el pipe2 para pasar la info al padre
				if(write(fds2[contador][1],listadoJuegosPorAnio,sizeof(juego)*cantidadJuegosPorAnio) == -1){
					printf("Error al escribir en el pipe");
					exit(1);
				}
				return 0;
				

			}else{
				// proceso padre
				// cerramos los pipes que no vamos a usar
				close(fds1[contador][0]);
				close(fds2[contador][1]);
				
				// Pasar la info al buffer
				int posicionFinalAnio=posicionesFinales[contador].posicionFinal;
				int anioLanzamiento=posicionesFinales[contador].anio;
				sprintf(buff, "%d,%d,%d\n", posicionInicioAnio,posicionFinalAnio,anioLanzamiento);
				posicionInicioAnio=posicionFinalAnio;

				// escribir en el pipe
				if(write(fds1[contador][1],buff, sizeof(buff)) == -1){
					printf("Error al escribir en el pipe");
					exit(1);
				}

				// esperar cada proceso
				waitpid(hijos_pid[contador],NULL,0);

				// leer del pipe 2
				if(read(fds2[contador][0],buff2,sizeof(buff2)) == -1){
					printf("Error al leer del pipe");
					exit(1);
				}
			}
		}

return 0;
}   