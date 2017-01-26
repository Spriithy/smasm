build:
	$(MAKE) -C asm build
	clang -Wall -O3 main.c **/*.c -o smasm
	mv smasm ../smasm

clean:
	$(MAKE) -C asm clean
	rm ../smasm
