

CC=gcc
CFLAGS=-I.
DEPS = serverHelpers.h estructuraDeDatos.h estructuraPromedios.h
OBJ = server.o serverHelpers.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server.cgi: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o server
