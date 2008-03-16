# $Id: Makefile,v 1.24 2008/03/16 00:47:10 laffer1 Exp $
#
#Flashback make file
#
#

CC=g++
CFLAGS= -pthread -Wall
BINNAME=bin/flashback
LIBS=-lsqlite3
OBJECTS=flashback.o fbCore.o fbThread.o fbCriticalSection.o fbLock.o fbErrorLogger.o \
	fbClient.o fbDiskDetector.o fbScheduler.o tcp.o fbSQL.o fbDate.o fbHttpServer.o

ifdef debug
CFLAGS += -DDebug -g
BINNAME=bin/flashback_d
else
CFLAGS += -O2
endif

ifdef win32
CFLAGS += -DWin32
ifdef DEBUG
BINNAME=bin\flashback_d.exe
else
BINNAME=bin\flashback.exe
endif
LIBS=-lws2_32
endif

#nix
all: clean release

release: flashback

debug: clean flashback
	
clean:
	rm -f bin/flashback bin/flashback_d bin/flashback.exe bin/flashback_d.exe *.o
	
#windows
win32:
	make -D WIN32 flashback

win32debug: clean flashback
	

#real part of the make
flashback: ${OBJECTS}
	mkdir -p bin
	${CC} ${CFLAGS} ${OBJECTS} ${LIBS} -o ${BINNAME}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $<

%.o: %.cpp %.h
	${CC} ${CFLAGS} -c -o $@ $<

%.o: sockets/%.c
	${CC} ${CFLAGS} -c -o $@ $<


