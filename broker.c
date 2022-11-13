#include "fbroker.h"

int main(int argc, char *argv[])
{
	// Semilla
	srand(time(NULL));
	// variables para los parametros de entrada
	char nombre_archivo_salida[100];
	strcpy(nombre_archivo_salida, argv[2]);
	char nombre_archivo_entrada[100];
	strcpy(nombre_archivo_entrada, argv[1]);
	char procesoPorPantalla[100];
	strcpy(procesoPorPantalla, argv[6]);
	int cantidadWorkers = atoi(argv[5]);

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
			// se cierran las entradas a los pipes inecesarias
			close(fd_padre_hijo[cont][1]);
			close(fd_hijo_padre[cont][0]);
			// dup para que el padre le escriba al hijo
			if (dup2(fd_padre_hijo[cont][0], STDIN_FILENO) == -1)
			{
				printf("Error al duplicar el descriptor de archivo");
				exit(1);
			}
			// dup para que el hijo le escriba al padre
			if (dup2(fd_hijo_padre[cont][1], STDOUT_FILENO) == -1)
			{
				printf("Error al duplicar el descriptor de archivo");
				exit(1);
			}
			// cierre de pipes dado que se realizo el dup
			close(fd_padre_hijo[cont][0]);
			close(fd_hijo_padre[cont][1]);
			// ejecucion de los procesos worker
			execlp("./worker", "worker",argv[3], argv[4], NULL);
		}
	}
	// cierre de entradas del pipe para el padre inecesarias
	for (int i = 0; i < cantidadWorkers; i++)
	{
		close(fd_padre_hijo[i][0]);
		close(fd_hijo_padre[i][1]);
	}
	// Se abre el archivo.
	FILE *archivo = fopen(nombre_archivo_entrada, "r");
	char buffer[2000];
	// lectura del archivo
	while (fgets(buffer, 2000, archivo) != NULL)
	{
		// se escoge un hijo de forma aleatoria
		int hijo = rand() % cantidadWorkers;
		// Se escribe la linea a el hijo
		if ((write(fd_padre_hijo[hijo][1], buffer, 2000)) == -1)
		{
			printf("Error al escribir en el pipe");
			exit(1);
		}
	}
	// Se escribe el FIN para que dejen de leer los workers
	for (int i = 0; i < cantidadWorkers; i++)
	{
		if ((write(fd_padre_hijo[i][1], "FIN", sizeof("FIN")) == -1))
		{
			printf("Error al escribir en el pipe");
			exit(1);
		}
	}
	// declaracion arreglo que contendra la cantidad de juegos por worker
	int *listadoCantidadJuegos = (int *)malloc(sizeof(int) * (cantidadWorkers));
	// declaracion arreglo que contendra la cantidad de anios por worker
	int *listadoCantidadAnios = (int *)malloc(sizeof(int) * (cantidadWorkers));
	// declaracion arreglo de lineas
	char **lineas = (char **)malloc(sizeof(char *) * (cantidadWorkers));
	// asignando memoria a el arreglo de lineas
	for (int i = 0; i < cantidadWorkers; i++)
	{
		lineas[i] = (char *)malloc(sizeof(char) * 20000);
	}
	// LECTURA DE LOS JUEGOS (LINEAS) LEIDAS POR CADA HIJO
	int cantidadJuegos;
	for (int i = 0; i < cantidadWorkers; i++)
	{
		read(fd_hijo_padre[i][0], &cantidadJuegos, sizeof(cantidadJuegos));
		listadoCantidadJuegos[i] = cantidadJuegos;
	}
	// LECTURA DE LOS ANIOS CALCULADOS POR CADA HIJO
	int cantidadAnios;
	for (int i = 0; i < cantidadWorkers; i++)
	{
		read(fd_hijo_padre[i][0], &cantidadAnios, sizeof(cantidadAnios));
		listadoCantidadAnios[i] = cantidadAnios;
	}

	// Matriz que contendra todos los calculos de cada anio por worker
	calculosJuegoPorAnio **calculosWorkers = (calculosJuegoPorAnio **)malloc(sizeof(calculosJuegoPorAnio *) * cantidadAnios);
	for (int i = 0; i < cantidadWorkers; i++)
	{
		calculosWorkers[i] = (calculosJuegoPorAnio *)malloc(sizeof(calculosJuegoPorAnio) * listadoCantidadAnios[i]);
	}

	// LECTURA CALCULOS WORKERS
	calculosJuegoPorAnio temp;
	for (int i = 0; i < cantidadWorkers; i++)
	{
		for (int j = 0; j < listadoCantidadAnios[i]; j++)
		{
			read(fd_hijo_padre[i][0], &temp, sizeof(calculosJuegoPorAnio));
			calculosWorkers[i][j] = temp;
		}
	}

	int cantidadAniosTotales = 0;
	// lista de anos
	int * listaAnios = aniosEnLista(calculosWorkers, cantidadWorkers, listadoCantidadAnios, &cantidadAniosTotales);
	
	// Arreglo que contendra todos los calculos de cada anio por worker.
	calculosJuegoPorAnio *totales = (calculosJuegoPorAnio*)malloc(sizeof(calculosJuegoPorAnio)*cantidadAniosTotales);
	int * banderas = (int *)calloc(cantidadAniosTotales, sizeof(int));
	for (int i = 0; i < cantidadWorkers; i++)
	{
		for (int j = 0; j < listadoCantidadAnios[i]; j++)
		{
			totales = evaluacionCalculo(totales, calculosWorkers[i][j],banderas,listaAnios,cantidadAniosTotales);
		}
	}

	// Se ordena el arreglo con los calculos finales de menor a mayor 
	mergeSort(totales, 0, cantidadAniosTotales-1);
	
	
	// ESCRITURA DEL ARCHIVO FINAL
	FILE *archivoSalida = fopen(nombre_archivo_salida, "w");
	for (int i = 0; i < cantidadAniosTotales; i++)
	{
		float porcentaje_win, porcentaje_mac, porcentaje_linuxx, promedioPrecios;
		porcentaje_win=((float)totales[i].windows/(float)totales[i].cantidadJuegos)*100;
		porcentaje_mac=((float)totales[i].mac/(float)totales[i].cantidadJuegos)*100;
		porcentaje_linuxx=((float)totales[i].linuxx/(float)totales[i].cantidadJuegos)*100;
		promedioPrecios=totales[i].sumaPreciosPorAnio/totales[i].cantidadJuegos;
		fprintf(archivoSalida,"ANIO: %d \n",totales[i].anioJuego);
		fprintf(archivoSalida,"Juego mas Caro: %s \n",totales[i].nombreJuegoMasCaro);
		fprintf(archivoSalida,"Juego mas Barato: %s \n",totales[i].nombreJuegoMasBarato);
		fprintf(archivoSalida,"Promedio de precios: %f \n",promedioPrecios);
		fprintf(archivoSalida,"Windows: %f Mac: %f Linux: %f \n",porcentaje_win,porcentaje_mac,porcentaje_linuxx);
		fprintf(archivoSalida,"Juegos gratis:\n %s \n",totales[i].juegosGratis);
	}
	fclose(archivoSalida);
	// IMPRESION POR PANTALLA EN CASO DE QUE EXISTA LA FLAG
	if (atoi(procesoPorPantalla) == 1)
	{
		for (int i = 0; i < cantidadAniosTotales; i++)
		{
			float porcentaje_win, porcentaje_mac, porcentaje_linuxx, promedioPrecios;
			porcentaje_win=((float)totales[i].windows/(float)totales[i].cantidadJuegos)*100;
			porcentaje_mac=((float)totales[i].mac/(float)totales[i].cantidadJuegos)*100;
			porcentaje_linuxx=((float)totales[i].linuxx/(float)totales[i].cantidadJuegos)*100;
			promedioPrecios = totales[i].sumaPreciosPorAnio/ totales[i].cantidadJuegos;
			printf("ANIO: %d \n", totales[i].anioJuego);
			printf("Juego mas Caro: %s \n", totales[i].nombreJuegoMasCaro);
			printf("Juego mas Barato: %s \n", totales[i].nombreJuegoMasBarato);
			printf("Promedio de precios: %f \n", promedioPrecios);
			printf("Windows: %f Mac: %f Linux: %f \n", porcentaje_win, porcentaje_mac, porcentaje_linuxx);
			printf("Juegos gratis:\n %s \n", totales[i].juegosGratis);
		}
		for (int i=0;i<cantidadWorkers; i++){
			printf("hijo %d ha leido: %d\n",i+1, listadoCantidadJuegos[i]);
		}
	}

	for (int i = 0; i < cantidadWorkers; i++)
	{
		free(lineas[i]);
	}
	free(lineas);
	for (int i = 0; i < cantidadWorkers; i++)
	{
		free(calculosWorkers[i]);
	}
	free(calculosWorkers);
	free(totales);
	return 0;
}