CC=gcc
CFLAGS=-I.
ODIR=obj
LIBS=-lm

DEPS = modelos.h
OBJ = main.o modelos.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

modelos: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o
