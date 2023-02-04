all: calc10 calc25

calc10: calc.o number10.o operation.o
	gcc calc.o number10.o operation.o -o calc10
calc25: calc.o number25.o operation.o
	gcc calc.o number25.o operation.o -o calc25

calc.o: calc.c number.h operation.h
	gcc -Wall -std=c99 -g -c calc.c

number10.o: number10.c number.h operation.h
	gcc -Wall -std=c99 -g -c number10.c
number25.o: number25.c number.h operation.h
	gcc -Wall -std=c99 -g -c number25.c

operation.o: operation.c operation.h
	gcc -Wall -std=c99 -g -c operation.c

clean:
	rm -f calc.o number10.o number25.o operation.o
	rm -f calc10 calc25
	rm -f output.txt