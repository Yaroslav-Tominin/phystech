all: ttt

ttt: starter.o
	gcc -o ttt $^ -fprofile-arcs -lm
CFLAGS:= -g -MD -fprofile-arcs -lm
-include *.d
clean:
	rm -rf *.o ttt
