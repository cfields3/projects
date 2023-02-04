agency: agency.o database.o input.o
	gcc agency.o database.o input.o -o agency

agency.o: agency.c database.h input.h
	gcc -Wall -std=c99 -g -c agency.c

database.o: database.c database.h input.h
	gcc -Wall -std=c99 -g -c database.c

input.o: input.c input.h
	gcc -Wall -std=c99 -g -c input.c

clean:
	rm -f agency.o database.o input.o
	rm -f agency
	rm -f output.txt
	rm -f stderr.txt