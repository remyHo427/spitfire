CC=gcc
CCFLAG=-Wall -Wextra -Werror -O3 -pedantic -std=c99
SRC=main.c lex.c parse.c ast.c

make: *.c
	$(CC) $(CCFLAG) $(SRC) -o spitfire
	./spitfire

test_lex: lex.c tlex.c
	$(CC) -Wall -Wextra -Werror -pedantic -std=c99 -g lex.c tlex.c -o a.out
	./a.out