# $Id: Makefile,v 1.19 2008/03/10 22:47:48 laffer1 Exp $
#
#Flashback make file
#
#

CC=g++
CFLAGS= -pthread -Wall
FILES=flashback.cpp fbCore.cpp fbThread.cpp fbCriticalSection.cpp fbLock.cpp fbErrorLogger.cpp fbSocket.cpp \
	fbClient.cpp sockets/tcp.c fbDiskDetector.cpp fbScheduler.cpp
#nix
BIND=bin/flashback_d
BINR=bin/flashback
#windows
WIN32=-DWin32
BINDw=bin\flashback_d.exe
BINRw=bin\flashback.exe
LIBw=-lws2_32

.if defined(DEBUG)
CFLAGS += -D Debug
.endif

#nix
all: clean release

release: flashback

debug: clean
	make -D DEBUG release
	
clean:
	rm -f ${BIND} ${BINR} *.o
	
#windows
win32: win32debug win32release
	
win32release: ${FILES}
	gmkdir -p bin
	${CC} ${FILES} ${WIN32} ${LIBw} -O3 -o ${BINRw}

win32debug: ${FILES}
	gmkdir -p bin
	${CC} ${FILES} ${WIN32} ${DEBUG} ${LIBw} -o ${BINDw}

win32clean:
	rm -f ${BINDw} ${BINRw} *.o
	rmdir bin

flashback: flashback.o fbCore.o fbThread.o fbCriticalSection.o fbLock.o fbErrorLogger.o fbClient.o tcp.o fbDiskDetector.o fbScheduler.o
	mkdir -p bin
	${CC} ${CFLAGS} flashback.o fbCore.o fbThread.o fbCriticalSection.o fbLock.o fbErrorLogger.o fbClient.o tcp.o fbDiskDetector.o fbScheduler.o -o ${BINR}

flashback.o: flashback.cpp global.h
	${CC} ${CFLAGS} -c flashback.cpp

fbCore.o: fbCore.cpp
	${CC} ${CFLAGS} -c fbCore.cpp

fbThread.o: fbThread.cpp fbThread.h
	${CC} ${CFLAGS} -c fbThread.cpp

fbCriticalSection.o: fbCriticalSection.cpp fbCriticalSection.h
	${CC} ${CFLAGS} -c fbCriticalSection.cpp

fbLock.o: fbLock.cpp fbLock.h
	${CC} ${CFLAGS} -c fbLock.cpp 

fbErrorLogger.o: fbErrorLogger.cpp fbErrorLogger.h
	${CC} ${CFLAGS} -c fbErrorLogger.cpp 

fbSocket.o: fbSocket.h fbSocket.cpp
	${CC} ${CFLAGS} -c fbSocket.cpp 

fbClient.o: fbClient.cpp fbClient.h
	${CC} ${CFLAGS} -c fbClient.cpp

tcp.o: sockets/tcp.c sockets/tcp.h
	${CC} ${CFLAGS} -c sockets/tcp.c 

fbDiskDetector.o: fbDiskDetector.cpp fbDiskDetector.h 
	${CC} ${CFLAGS} -c fbDiskDetector.cpp

fbScheduler.o: fbScheduler.cpp fbScheduler.h
	${CC} ${CFLAGS} -c fbScheduler.cpp

