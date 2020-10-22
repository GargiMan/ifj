# File: Makefile
# Author: Marek Gergel (xgerge01)
# Brief: IFJ20 Compiler

C = gcc -std=c11 -Wall -Wextra -pedantic -lm
OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
PROG = ifj20
LOGIN = xjacol00

.PHONY: clean

all: $(PROG)

%.o: %.c
	$(C) -c $<

$(PROG): $(OBJ)
	$(C) $^ -o $@

check: zip
	./is_it_ok.sh $(LOGIN).zip $(LOGIN)

clean:
	rm -rf *.o $(PROG) $(LOGIN)*

zip:
	zip -q $(LOGIN).zip *.c *.h Makefile dokumentace.pdf rozdeleni rozsireni
