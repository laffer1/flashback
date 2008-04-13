# $Id: Makefile,v 1.51 2008/04/13 20:27:10 laffer1 Exp $
#
#Flashback make file
#
#

CC=g++
CFLAGS= -pthread -Wall -pedantic
BINNAME=bin/flashback
LIBS=-lsqlite3 -larchive -lbz2 -lz
OBJECTS=flashback.o fbCore.o fbThread.o fbCriticalSection.o fbLock.o fbErrorLogger.o \
	fbClient.o fbDiskDetector.o fbScheduler.o tcp.o fbSQL.o fbDate.o fbHttpServer.o \
	fbSocket.o fbTime.o fbDatabase.o fbData.o fbHttpResponse.o fbConfig.o \
	fbBackup.o fbRestore.o urldecode.o

PREFIX= /usr/local
DESTDIR=
DOCROOT= ${PREFIX}/share/flashback
DATABASEROOT= /var/flashback

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
	make flashback DEBUG=yes

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

install:
	@echo "Installing Flashback"
	mkdir -p ${DESTDIR}${DATABASEROOT}
	mkdir -p ${DESTDIR}${DOCROOT}
	cp -R www ${DESTDIR}${DOCROOT}
	cp README  ${DESTDIR}${DOCROOT}
	cp flashback.db ${DESTDIR}${DATABASEROOT}
	cp bin/flashback ${DESTDIR}${PREFIX}/sbin/flashback
	chmod 755 ${DESTDIR}${PREFIX}/sbin/flashback
#	cp flashback.sh ${DESTDIR}${PREFIX}/etc/rc.d/flashback.sh
#	chmod 755 ${DESTDIR}${PREFIX}/etc/rc.d/flashback.sh

dinstall: uninstall
	@echo "Installing Flashback Debug Version"
	mkdir -p ${DESTDIR}${DATABASEROOT}
	mkdir -p ${DESTDIR}${DOCROOT}
	cp -R www ${DESTDIR}${DOCROOT}
	cp README  ${DESTDIR}${DOCROOT}
	cp flashback.db ${DESTDIR}${DATABASEROOT}
	cp bin/flashback_d ${DESTDIR}${PREFIX}/sbin/flashback
	chmod 755 ${DESTDIR}${PREFIX}/sbin/flashback
#	cp flashback.sh ${DESTDIR}${PREFIX}/etc/rc.d/flashback.sh
#	chmod 755 ${DESTDIR}${PREFIX}/etc/rc.d/flashback.sh

uninstall:
	rm -rf ${DESTDIR}${DATABASEROOT}
	rm -rf ${DESTDIR}${DOCROOT}
	rm -f ${DESTDIR}${PREFIX}/sbin/flashback
	rm -f /var/log/flashback
#	rm -f ${DESTDIR}${PREFIX}/etc/rc.d/flashback.sh

