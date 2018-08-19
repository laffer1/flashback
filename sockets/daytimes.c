/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An example of a TCP datetimed (server process)
and retrieves the date and time.  This does not use
the standard C routines, but instead calls a library i wrote
to handle the TCP traffic.  

We do not fork each child process because the request can be handled so quickly.
It would be possible to fork a child after using tcpserverclient() with the 
fd it gives you. :)

Based on an example from the FreeBSD developers handbook.
The chapter starts here
http://www.freebsd.org/doc/en_US.ISO8859-1/books/developers-handbook/sockets.html

Datetype protocol reference
http://www.faqs.org/rfcs/rfc867.html
*/

/* for fdopen to work */
#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include "tcp.h"

int main( void )
{
    socketdesc sd; /* similar to a file desc, part of my tcp library */
    int sockfd;    /* individual connection to process */
    FILE *client;  /* write to client (c stream) */
    time_t t;      /* the current time */
    struct tm *tm; /* time after its broken up into its components */

    inittcp();
    
    /* 0.0.0.0 should be all interfaces on a unix/linux host */
    if ( (sd = opentcp( true, "0.0.0.0", 13 )) < 0 )
    {
        fprintf( stderr, "Error opening the connection\n" );
        exit(1);
    }

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


    /* servers run in infinite loops! */
    while (1) 
    {
        /* fetch the client's socket file descriptor! */
        sockfd = tcpserverclient( sd );

        if ( sockfd == ETCPACCEPTFAIL )
        {
            perror( "daytimes accept" );
            exit(1);
        }

        /* open it in a C stream */
        if ( (client = fdopen(sockfd, "w")) == NULL ) 
        {
            perror("daytimes fdopen");
            exit(1);
        }

        /* get the current time */
        if ((t = time(NULL)) < 0) 
        {
            perror("daytimes time");
            exit(1);
        }

        /* break it up into useful pieces and print it */
        tm = gmtime(&t);
        fprintf(client, "%.4i-%.2i-%.2iT%.2i:%.2i:%.2iZ\n",
            tm->tm_year + 1900,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);

        fclose(client);  /* closes the client socket file descriptor */
    }

    closetcp( sd ); /* closes the tcp connection.. never reached.. */
}
