/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An example of a TCP client that connects to a daytime server
and retrieves the date and time.  This does not use
the standard C routines, but instead calls a library i wrote
to handle the TCP traffic.  

Based on an example from the FreeBSD developers handbook.
The chapter starts here
http://www.freebsd.org/doc/en_US.ISO8859-1/books/developers-handbook/sockets.html

daytime protocol
http://www.faqs.org/rfcs/rfc867.html
*/

#include <stdio.h>
#include <stdlib.h> 
#include "tcp.h"

int main( int argc, char *argv[] )
{
    char *host;    /* hostname, domain name or ip address to connect to */
    socketdesc sd; /* similar to a file desc, part of my tcp library */
    char *output;  /* a temp pointer to output.  */

    host = (argc > 1) ? (char *)argv[1] : "time.nist.gov";

    inittcp(); /* required the first time the library is used. */
    if ( (sd = opentcp( FALSE, host, 13 )) < 0 )
    {
        fprintf( stderr, "Error opening the connection\n" );
        exit(1);
    }

    while ((output = readtcp( sd )) != NULL )
    {
        printf( "%s", output );
        free( output );  /* be a good citizen and free the memory we used */
    }

    closetcp( sd );

    return 0;
}
