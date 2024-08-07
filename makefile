CC=gcc
WFLAG=-Wall -Wextra -Werror
NDFLAG=-DNDEBUG_MEM -DNDEBUG
CCFLAG=-pedantic -std=c99
SRC=main.c lex.c parse.c ast.c arena.c

make: *.c
	$(CC) $(CCFLAG) $(WFLAG) $(SRC) $(NDFLAG) -O3 -o spitfire
	./spitfire

test_lex: lex.c tlex.c
	$(CC) $(WFLAG) $(CCFLAG) -O0 -g lex.c tlex.c -o a.out
	./a.out

test_parse: dmem.c lex.c parse.c tparse.c ast.c
	$(CC) $(WFLAG) $(CCFLAG) -O0 -DMEM_DEBUG_QUIET -g \
		dmem.c lex.c parse.c tparse.c ast.c -o a.out
	./a.out