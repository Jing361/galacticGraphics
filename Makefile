CC:=g++
flags:=-Wall -Wextra -pedantic -std=c++14
dflags:=-ggdb3
cflags:=-c
iflags:=-Iinclude/ -Iinclude/gl3w/include -Iinclude/glfw/include -Iinclude/glm

name:=main

default:$(name)

$(name):$(name).o fileloader.o
	$(CC) $(name).o fileloader.o -o $(name)

$(name).o:$(name).cc
	$(CC) $(cflags) $(iflags) $(flags) $(name).cc

fileloader.o:fileloader.cc
	$(CC) $(cflags) $(iflags) $(flags) fileloader.cc

clean:
	rm *.a *.so *.o core $(name)

