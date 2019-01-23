CC=clang++
CCFLAGS=--std=c++11

all: bfi bfgen

bfi: bfi.cc
	${CC} ${CCFLAGS} bfi.cc -o bfi

bfgen: bfgen.cc
	${CC} ${CCFLAGS} bfgen.cc -o bfgen

clean:
	-rm bfi bfgen examples/*.exe examples/*.c
