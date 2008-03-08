/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An example of a UDP echo server. This does not use
the standard C routines, but instead calls a library I wrote
to handle the UDP traffic.  

This happens to run on port 2007 but real echo uses port 7.
*/

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

#include "udp.h"

#define MSGSIZE 1024

int main( void )
{
    socketdesc sd;     /* socket descriptor index */
    char msg[MSGSIZE]; /* send/recv buffer */
    char * addr;       /* address we are send/reciving to */

    /* this is done so we get back the terminal */
    switch (fork()) 
    {
        case -1:
            perror("fork");
            exit(1);
            break;
        
        case 0:     /* child */
            break;

        default:   /* parent */
            return 0;
            break;
    }

    initudp();
    sd = openudp( TRUE, "127.0.0.1", 2007 );

    while (1)
    {
        recvudp( sd, msg, MSGSIZE - 1, addr );
        sendudp( sd, msg, MSGSIZE - 1, addr );
    }

    closeudp( sd );

    return 0;
}
