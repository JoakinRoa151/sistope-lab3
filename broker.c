#include "fbroker.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));
	/*printf("Esta es i: %s\n",argv[1]);
	printf("Esta es o: %s\n",argv[2]);
	printf("Esta es d: %s\n",argv[3]);
	printf("Esta es p: %s\n",argv[4]);
	printf("Esta es n: %s\n",argv[5]);
	printf("Esta es b: %s\n",argv[6]);*/

	// conversion de los argumentos a enteros
	// int anio = atoi(argv[3]);
	// int precioMenor = atoi(argv[4]);
	char nombre_archivo_salida[100];
	strcpy(nombre_archivo_salida, argv[2]);
	char nombre_archivo_entrada[100];
	strcpy(nombre_archivo_entrada, argv[1]);
	char procesoPorPantalla[100];
	strcpy(procesoPorPantalla, argv[6]);
	int cantidadWorkers = atoi(argv[5]);

	// printf("anio: %d\n", anio);
	// printf("precio menor: %d\n", precioMenor);
	// printf("Nombre archivo salida: %s\n", nombre_archivo_salida);
	// printf("Nmbre archivo entrada: %s\n", nombre_archivo_entrada);
	// printf("workers: %d\n", cantidadWorkers);
	// printf("bflag: %s\n", procesoPorPantalla);

	// pid de los procesos hijos;
	int workers_pid[cantidadWorkers];

	// pipes•••••••••••
	int fd_padre_hijo[cantidadWorkers][2]; // padre escritura 1
	int fd_hijo_padre[cantidadWorkers][2]; // padre lectura 0
	// cantidad de pipes por proceso que es igual al numero de workers

	for (int cont = 0; cont < cantidadWorkers; cont++)
	{
		pipe(fd_padre_hijo[cont]);
		pipe(fd_hijo_padre[cont]);
	}
	// creacion de los procesos hijos
	for (int cont = 0; cont < cantidadWorkers; cont++)
	{
		workers_pid[cont] = fork();
		// condición para cuando existe un error el proceso hijo.
		if (workers_pid[cont] == -1)
		{
			printf("Error al crear el proceso hijo");
			exit(1);
		}
		else if (workers_pid[cont] == 0)
		{
			// proceso hijo
			close(fd_padre_hijo[cont][1]);
			close(fd_hijo_padre[cont][0]);
			if (dup2(fd_padre_hijo[cont][0], STDIN_FILENO) == -1)
			{
				printf("Error al duplicar el descriptor de archivo");
				exit(1);
			}
			if (dup2(fd_hijo_padre[cont][1], STDOUT_FILENO) == -1)
			{
				printf("Error al duplicar el descriptor de archivo");
				exit(1);
			}
			close(fd_padre_hijo[cont][0]);
			close(fd_hijo_padre[cont][1]);
			execlp("./worker", "worker", NULL);
		}
	}

	for (int i = 0; i < cantidadWorkers; i++)
	{
		close(fd_padre_hijo[i][0]);
		close(fd_hijo_padre[i][1]);
	}
	char linea[1000];
	// Se abre el archivo.
	FILE *archivo = fopen(nombre_archivo_entrada, "r");
	char buffer[1000];
	while (fgets(buffer, 1000, archivo) != NULL)
	{
		// ESCRITURA PADRE
		int hijo = rand() % cantidadWorkers;
		strcpy(linea, buffer);
		if ((write(fd_padre_hijo[hijo][1], linea, 1000)) == -1)
		{
			printf("Error al escribir en el pipe");
			exit(1);
		}
	}

	for (int i = 0; i < cantidadWorkers; i++)
	{
		if ((write(fd_padre_hijo[i][1], "FIN", sizeof("FIN")) == -1)){
			printf("Error al escribir en el pipe");
			exit(1);
		}
	}
	int lenListadoCalculos=0;
	calculosJuegoPorAnio * listadoCalculos;
	int condicion;
	char buffer2[11000];
	calculosJuegoPorAnio calculoPorAnio;
	for (int i = 0; i < cantidadWorkers; i++)
	{
		condicion=1;
		while (condicion == 1)
		{
			read(fd_hijo_padre[i][0], buffer2, 11000);
			if(strcmp(buffer2,"FIN")==0){
				condicion=0;
				printf(" \n TERMINOOO \n\n");
			}
			//printf("\n %s \n", buffer2);
			else{
				printf("\n llego? \n");
				calculoPorAnio= lecturaCalculo(buffer2);
				printf("%d %d %d %d %d %s %s %s %f %f %f", calculoPorAnio.anioJuego, calculoPorAnio.cantidadJuegos, calculoPorAnio.windows, calculoPorAnio.mac, calculoPorAnio.linuxx, calculoPorAnio.nombreJuegoMasCaro, calculoPorAnio.nombreJuegoMasBarato, calculoPorAnio.juegosGratis, calculoPorAnio.precioJuegoMasCaro, calculoPorAnio.precioJuegoMasBarato, calculoPorAnio.sumaPreciosPorAnio);

			}
		}
	}
	return 0;
}