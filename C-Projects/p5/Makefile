all: encode decode

encode: encode.o state24.o filebuffer.o
	gcc encode.o state24.o filebuffer.o -o encode

decode: decode.o state24.o filebuffer.o
	gcc decode.o state24.o filebuffer.o -o decode

encode.o: encode.c state24.h filebuffer.h
	gcc -Wall -std=c99 -g -c encode.c

decode.o: decode.c state24.h filebuffer.h
	gcc -Wall -std=c99 -g -c decode.c

state24.o: state24.c state24.h filebuffer.h
	gcc -Wall -std=c99 -g -c state24.c

filebuffer.o: filebuffer.c filebuffer.h
	gcc -Wall -std=c99 -g -c filebuffer.c

clean:
	rm -f encode.o decode.o state24.o filebuffer.o
	rm -f encode
	rm -f decode
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt