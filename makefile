CC=gcc
CCFLAG=-Wall -Wextra -Werror -O3 -pedantic -std=c99

make: *.c
	$(CC) $(CCFLAG) main.c lex.c -o spitfire
	./spitfire

test_lex: lex.c tlex.c
	$(CC) -Wall -Wextra -Werror -pedantic -std=c99 -g lex.c tlex.c -o a.out
	./a.out