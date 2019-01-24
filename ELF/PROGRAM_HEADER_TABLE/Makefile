
.PHONY= clean


all: pht pht32

pht: pht.c
	gcc -no-pie $< -o $@ 

pht32: pht.c
	gcc -m32 -no-pie $< -o $@

clean:
	rm pht pht32
