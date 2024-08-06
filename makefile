CC=gcc
WFLAG=-Wall -Wextra -Werror
NDFLAG=-DNDEBUG_MEM -DNDEBUG
CCFLAG=-O3 -pedantic -std=c99
SRC=main.c lex.c parse.c

make: *.c
	$(CC) $(CCFLAG) $(WFLAG) $(SRC) $(NDFLAG) -o spitfire
	./spitfire

test_lex: lex.c tlex.c
	$(CC) $(WFLAG) $(CCFLAG) -g lex.c tlex.c -o a.out
	./a.out