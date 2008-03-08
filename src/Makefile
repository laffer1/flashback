#Flashback make file
#
#

CC=g++
FILES=flashback.cpp fbThread.cpp fbCriticalSection.cpp fbErrorLogger.cpp
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

release: flashback.cpp fbThread.cpp fbCriticalSection.cpp fbErrorLogger.cpp
	mkdir -p bin
	${CC} ${FILES} -O3 -o ${BINR}

debug: flashback.cpp fbThread.cpp fbCriticalSection.cpp fbErrorLogger.cpp
	mkdir -p bin
	${CC} ${FILES} ${DEBUG} -o ${BIND}
	
clean:
	rm -f ${BIND} ${BINR}
	rmdir bin
	
#windows
win32: win32debug win32release
	
win32release: flashback.cpp fbThread.cpp fbCriticalSection.cpp fbErrorLogger.cpp
	gmkdir -p bin
	${CC} ${FILES} ${WIN32} -O3 -o ${BINRw}

win32debug: flashback.cpp fbThread.cpp fbCriticalSection.cpp fbErrorLogger.cpp
	gmkdir -p bin
	${CC} ${FILES} ${WIN32} ${DEBUG} -o ${BINDw}

win32clean:
	rm -f ${BINDw} ${BINRw}
	rmdir bin
	
