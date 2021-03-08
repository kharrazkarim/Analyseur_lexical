
CC=gcc
INCLUDE= -I./
CFLAGS=-g $(INCLUDE)
all: clean compil

compil: 
	$(CC) $(CFLAGS) main.c bib.c -o compil $<
run:compil
	chmod u+x compil
	./compil
debug:
	gdb ./compil
clean:
	rm -f compil
