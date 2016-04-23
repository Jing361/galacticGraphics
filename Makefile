CC:=g++
flags:=-Wall -Wextra -pedantic -std=c++14
dflags:=-ggdb3
cflags:=-c $(dflags)

name:=main

default:$(name)

$(name):$(name).o
	$(CC) $(name).o -o $(name)

$(name).o:$(name).cc
	$(CC) $(cflags) $(flags) $(name).cc

clean:
	rm *.a *.so *.o core $(name)

