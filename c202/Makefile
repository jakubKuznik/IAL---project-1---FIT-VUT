
PRJ=c202
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

.PHONY: test
	./$(PROGS) > test.out
	diff $(PRJ) -test.output test.out 


clean:
	rm -f *.o *.output $(PROGS)
#
