driver: driver.o input.o map.o vtype.o integer.o text.o
	gcc driver.o input.o map.o vtype.o integer.o text.o -o driver

driver.o: driver.c input.h map.h vtype.h integer.h text.h
	gcc -Wall -std=c99 -g -c driver.c

input.o: input.c input.h
	gcc -Wall -std=c99 -g -c input.c

map.o: map.c map.h vtype.h
	gcc -Wall -std=c99 -g -c map.c

integer.o: integer.c integer.h vtype.h
	gcc -Wall -std=c99 -g -c integer.c

text.o: text.c text.h vtype.h
	gcc -Wall -std=c99 -g -c text.c

vtype.o: vtype.c vtype.h
	gcc -Wall -std=c99 -g -c vtype.c

clean:
	rm -f driver.o input.o map.o vtype.o integer.o text.o
	rm -f driver
	rm -f output.txt
	rm -f stderr.txt