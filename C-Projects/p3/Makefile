wordle: wordle.o lexicon.o io.o history.o
	gcc wordle.o history.o lexicon.o io.o -o wordle

wordle.o: wordle.c lexicon.h io.h history.h
	gcc -Wall -std=c99 -g -c wordle.c

lexicon.o: lexicon.c lexicon.h io.h
	gcc -Wall -std=c99 -g -c lexicon.c

io.o: io.c io.h
	gcc -Wall -std=c99 -g -c io.c

history.o: history.c history.h
	gcc -Wall -std=c99 -g -c history.c

clean:
	rm -f wordle.o lexicon.o io.o history.o
	rm -f wordle
	rm -f output.txt