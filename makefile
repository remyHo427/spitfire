CC=gcc
WFLAG=-Wall -Wextra -Werror
NDFLAG=-DNDEBUG_MEM -DNDEBUG
CCFLAG=-pedantic -std=c99
SRC=arena.c lex.c main.c parse.c

make: *.c
	$(CC) $(CCFLAG) $(NDFLAG) $(SRC) -O3 -o spitfire
	./spitfire

run: *.c
	$(CC) $(CCFLAG) $(WFLAG) -DMEM_DEBUG_QUIET *.c -O0 -g -o spitfire
	./spitfire