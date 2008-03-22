# $Id: Makefile,v 1.34 2008/03/22 23:48:41 laffer1 Exp $
#
#Flashback make file
#
#

CC=g++
CFLAGS= -pthread -Wall -pedantic
BINNAME=bin/flashback
LIBS=-lsqlite3
OBJECTS=flashback.o fbCore.o fbThread.o fbCriticalSection.o fbLock.o fbErrorLogger.o \
	fbClient.o fbDiskDetector.o fbScheduler.o tcp.o fbSQL.o fbDate.o fbHttpServer.o \
	fbSocket.o fbTime.o fbDatabase.o fbData.o fbHttpResponse.o fbConfig.o

ifdef DEBUG
CFLAGS += -DDebug -g
BINNAME=bin/flashback_d
else
CFLAGS += -O2
endif

ifdef WIN32
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

debug:
	gmake flashback DEBUG=yes

clean:
	rm -f bin/flashback bin/flashback_d bin/flashback.exe bin/flashback_d.exe *.o

#windows
win32:
	gmake flashback WIN32=yes

win32debug:
	gmake flashback WIN32=yes DEBUG=yes


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


