CC=g++
CFLAGS=-g -I./evmjit/libevmjit -I./evmjit/include -I./evmjit/evmc/include
LDFLAGS=-L./evmjit/build/libevmjit/ -levmjit-standalone -lpthread

%.o:	%.cc
	$(CC) -c -o $@ $< $(CFLAGS)

lift:	lift.o
	$(CC) -o $@ lift.o $(LDFLAGS)

all:	lift

clean:
	rm -f lift *.o
