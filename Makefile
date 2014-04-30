CC=gcc
CFLAGS=-I.
DEPS = helper.h
OBJ = main.o helper.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f main *.o
