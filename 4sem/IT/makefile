all: ttt

ttt: example.o tree.o
	gcc -o ttt $^ -fprofile-arcs -ftest-coverage
CFLAGS:= -g -MD -fprofile-arcs -ftest-coverage
-include *.d
clean:
	rm -rf *.o ttt
