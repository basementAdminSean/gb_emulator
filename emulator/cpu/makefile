CXX = gcc -Wall -ansi -pedantic -std=c23 -g

decode: decode.o
	$(CXX) -o decode $^

decode.o: decode.c

clean: 
	rm -f *.o decode 
