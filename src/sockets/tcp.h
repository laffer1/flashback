/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An library to abstract the C sockets interface from the programmer.  This
file describes function prototypes for the TCP protocol implementation.  
*/

#ifndef TCPH
#define TCPH

#include "bool.h"

/* library errors */
#define ETCPACCEPTFAIL -7
#define ETCPLISTENFAIL -6
#define ETCPBINDFAIL   -5
#define ETCPCONNFAIL   -4
#define ETCPBADADDRESS -3
#define ETCPMAXCONS    -2
#define ETCPGENERIC    -1
/* end errors */

typedef int socketdesc; /* similar to a file descriptor.. an index for
                           created connections */

void inittcp( void );
socketdesc opentcp( bool server, char * address, int port );
void closetcp( socketdesc sd );
char * readtcp( socketdesc sd );
char * readntcp( socketdesc sd, int size );
int tcpserverclient( socketdesc sd );
int tcpclientfd( socketdesc sd );
int writetcp( socketdesc sd, char * str );
int writentcp( socketdesc sd, char * str, int len );
int tcpputs( socketdesc sd, char * str );

#endif
