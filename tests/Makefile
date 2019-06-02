exec = test.out
sources = $(wildcard src/*.c)
sources += $(wildcard ../src/*.c)
objects = $(sources:.c=.o)
flags = -g -I../../cmocka/include -ldl -lm -lcmocka


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
