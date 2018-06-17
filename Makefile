CC=gcc
CFLAGS=-I.
LIBS=-lm

DEPS = modelos.h lqrController.h pidController.h
OBJ = main.o modelos.o lqrController.o pidController.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o
