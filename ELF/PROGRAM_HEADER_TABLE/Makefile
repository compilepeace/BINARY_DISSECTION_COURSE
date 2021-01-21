
.PHONY= clean


all: parse_pht pht pht32 

parse_pht: parse_pht.c
	gcc $< -o $@

pht: pht.c
	gcc -no-pie $< -o $@ 

pht32: pht.c
	gcc -m32 -no-pie $< -o $@

clean:
	rm pht pht32 parse_pht
