CC = gcc
CCFLAGS = -Wall -ansi -pedantic -std=c23 -g

decode: decode.o
	 $(CC) -o decode decode.o

decode.o: decode.c
	$(CC) $(CCFLAGS) -c decode.c

clean: 
	rm -f *.o decode 
