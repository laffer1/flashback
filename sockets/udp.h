/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An library to abstract the C sockets interface from the programmer.  This
file describes function prototypes for the UDP protocol implementation.     

*/


#ifndef UDPH
#define UDPH

#include "bool.h"

typedef int socketdesc; /* similar to a file descriptor.. an index for
                           created connections */

void initudp( void );
socketdesc openudp( bool server, char * address, int port );
void closeudp( socketdesc sd );
long int sendudp( socketdesc sd, char * msg, int len, char * address );
long int recvudp( socketdesc sd, char * msg, int len, char * address );

/* library errors */
#define EUDPBINDFAIL   -5
#define EUDPCONNFAIL   -4
#define EUDPBADADDRESS -3
#define EUDPMAXCONS    -2
#define EUDPGENERIC    -1
/* end errors */

#endif
