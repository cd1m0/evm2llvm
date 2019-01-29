CC=g++
CFLAGS=-g -I./evmjit/libevmjit -I./evmjit/include -I./evmjit/evmc/include
LDFLAGS=-L./evmjit/build/libevmjit/ -levmjit-standalone -lpthread

%.o:	%.cc
	$(CC) -c -o $@ $< $(CFLAGS)

evmjit/build/libevmjit/libevmjit-standalone.a:
	[ -d evmjit/build ] || mkdir evmjit/build
	cd evmjit/build && cmake ..
	cd evmjit/build && cmake --build . --config RelWithDebInfo


libevmjit:	evmjit/build/libevmjit/libevmjit-standalone.a

lift:	lift.o libevmjit
	$(CC) -o $@ lift.o $(LDFLAGS)

all:	lift

clean:
	rm -f lift *.o
