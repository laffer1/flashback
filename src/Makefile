# $Id: Makefile,v 1.13 2008/03/09 08:49:47 laffer1 Exp $
#
#Flashback make file
#
#

CC=g++
CFLAGS=	-pthread -Wall
FILES=flashback.cpp fbThread.cpp fbCriticalSection.cpp fbLock.cpp fbErrorLogger.cpp fbSocket.cpp fbClient.cpp sockets/tcp.c
DEBUG=-DDebug
#nix
BIND=bin/flashback_d
BINR=bin/flashback
#windows
WIN32=-DWin32
BINDw=bin\flashback_d.exe
BINRw=bin\flashback.exe

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
	${CC} ${FILES} ${WIN32} -O3 -o ${BINRw}

win32debug: ${FILES}
	gmkdir -p bin
	${CC} ${FILES} ${WIN32} ${DEBUG} -o ${BINDw}

win32clean:
	rm -f ${BINDw} ${BINRw}
	rmdir bin
	
