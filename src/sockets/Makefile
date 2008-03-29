#author: Lucas Holt & Rupali Y Mahajan
#date due: April 21, 2005
#assignment: Documentation Project, TCP/UDP Sockets
#class: cs224
#class time: 11am MWF

CC=g++
CFLAGS=-Wall 

all: clean tcp.o udp.o daytimec daytimes echos echoc

echos: udp.o echos.c
	${CC} ${CFLAGS} udp.o echos.c -o echos

echoc: udp.o echoc.c
	${CC} ${CFLAGS} udp.o echoc.c -o echoc

daytimes: tcp.o daytimes.c
	${CC} ${CFLAGS} tcp.o daytimes.c -o daytimes

daytimec: tcp.o daytimec.c
	${CC} ${CFLAGS} tcp.o daytimec.c -o daytimec

tcp.o: tcp.h tcp.c
	${CC} ${CFLAGS} -c tcp.c

udp.o: udp.h udp.c
	${CC} ${CFLAGS} -c udp.c

clean:
	rm -f *.o daytimec daytimes echos echoc
