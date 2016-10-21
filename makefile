all: finder

finder:   queue.o prod_cons.o
	gcc -pthread -o finder  prod_cons.o

queue.o: queue.c
	gcc  -pthread -o queue.o -c queue.c -W -Wall

prod_cons.o: prod_cons.c queue.o
	gcc -pthread -o prod_cons.o  -c prod_cons.c -W -Wall

clean:
	rm  *.o parser
