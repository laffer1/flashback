/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An example of a UDP echo server. This does not use
the standard C routines, but instead calls a library I wrote
to handle the UDP traffic.  

Our test server uses port 2007 so our client is set to that as well.
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "udp.h"

#define MSGSIZE 1024

int main( int argc, char * argv[] )
{
    socketdesc sd;         /* socket descriptor index */
    long n;                /* number of bytes received */
    char sendline[MSGSIZE];   /* send buffer */
    char recvline[MSGSIZE+1]; /* recieve buffer */

    /* need an ip address.. localhost or 127.0.0.1 would work for
       our test case */
    if (argc != 2 )
    {
        fprintf( stderr, "echo <ip address>\n" );
        exit(1);
    }

    initudp();
    sd = openudp( FALSE, argv[1], 2007 );

    if ( sd < 0 )
    {
        fprintf( stderr, "error occured while connecting\n" );
        exit(1);
    }

    while ( fgets( sendline, MSGSIZE, stdin ) != NULL )
    {
        sendudp( sd, sendline, strlen(sendline), argv[1] );
      
        n = recvudp( sd, recvline, MSGSIZE, NULL );
        recvline[n] = '\0';
        fputs(recvline, stdout);
    }

    closeudp( sd );

    return 0;
}
