CFLAGS= -no-pie


all: main parse_sht

main: main.c
	gcc $(CFLAGS) main.c -o main 

parse_sht: parse_sht.c
	gcc $< -o $@

clean:
	rm main parse_sht
