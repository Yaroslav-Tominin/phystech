all: ttt

ttt: 2.o 1.o
	gcc -o ttt $^
CFLAGS:= -g -MD
-include *.d
clean:
	rm -rf *.o ttt
