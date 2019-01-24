


CFLAGS_64= -no-pie
CFLAGS_32= -m32 -no-pie


all: hello_32 hello_64


hello_32: hello.c
	gcc $(CFLAGS_32) $< -o $@ 
	
hello_64: hello.c
	gcc $(CFLAGS_64) $< -o $@ 

clean: 
	rm hello_32 hello_64 *.o
