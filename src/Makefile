# $Id: Makefile,v 1.20 2008/03/11 00:14:15 laffer1 Exp $
#
#Flashback make file
#
#

CC=g++
CFLAGS= -pthread -Wall
BINNAME=bin/flashback
LIBS=

.if defined(DEBUG)
CFLAGS += -D Debug -g
BINNAME=bin/flashback_d
.else
CFLAGS += -O2
.endif

.if defined(WIN32)
CFLAGS += -D Win32
.if defined(DEBUG)
BINNAME=bin\flashback_d.exe
.else
BINNAME=bin\flashback.exe
.endif
LIBS=-lws2_32
.endif

#nix
all: clean release

release: flashback

debug: clean
	make -D DEBUG release
	
clean:
	rm -f bin/flashback bin/flashback_d bin/flashback.exe bin/flashback_d.exe *.o
	
#windows
win32:
	make -D WIN32 flashback

win32debug: clean
	make -D WIN32 -D DEBUG flashback

flashback: flashback.o fbCore.o fbThread.o fbCriticalSection.o fbLock.o fbErrorLogger.o fbClient.o tcp.o fbDiskDetector.o fbScheduler.o
	mkdir -p bin
	${CC} ${CFLAGS} flashback.o fbCore.o fbThread.o fbCriticalSection.o fbLock.o fbErrorLogger.o fbClient.o tcp.o fbDiskDetector.o fbScheduler.o ${LIBS} -o ${BINNAME}

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

