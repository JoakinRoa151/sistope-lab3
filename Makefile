FLAGS = -Wall -lm
main: lab2.c broker worker
	gcc $(FLAGS) -o lab2 lab2.c

broker: broker.c fbroker.o
	gcc $(FLAGS) -o broker broker.c fbroker.o

worker: worker.c fworker.o
	gcc $(FLAGS) -o worker worker.c fworker.o

fbroker.o: fbroker.c
	gcc -c $(FLAGS) fbroker.c

fworker.o: fworker.c
	gcc -c $(FLAGS) fworker.c

clear:
	rm -f *.o main
