# $Id: Makefile,v 1.15 2008/03/10 19:34:08 wyverex Exp $
#
#Flashback make file
#
#

CC=g++
CFLAGS=	-pthread -Wall
FILES=flashback.cpp fbCore.cpp fbThread.cpp fbCriticalSection.cpp fbLock.cpp fbErrorLogger.cpp fbSocket.cpp fbClient.cpp sockets/tcp.c
DEBUG=-DDebug
#nix
BIND=bin/flashback_d
BINR=bin/flashback
#windows
WIN32=-DWin32
BINDw=bin\flashback_d.exe
BINRw=bin\flashback.exe
LIBw=-lws2_32

#nix
all: debug release

release: ${FILES}
	mkdir -p bin
	${CC} ${CFLAGS} ${FILES} -O2 -o ${BINR}

debug: ${FILES}
	mkdir -p bin
	${CC} ${CFLAGS} ${FILES} ${DEBUG} -o ${BIND}
	
clean:
	rm -f ${BIND} ${BINR}
	rmdir bin
	
#windows
win32: win32debug win32release
	
win32release: ${FILES}
	gmkdir -p bin
	${CC} ${FILES} ${WIN32} ${LIBw} -O3 -o ${BINRw}

win32debug: ${FILES}
	gmkdir -p bin
	${CC} ${FILES} ${WIN32} ${DEBUG} ${LIBw} -o ${BINDw}

win32clean:
	rm -f ${BINDw} ${BINRw}
	rmdir bin
	
