/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An library to abstract the C sockets interface from the programmer.  This
file implements the TCP protocol.  Although everything is well documented,
you can start by looking at inittcp() and opentcp().   


bugs:
    the cons socket descriptor table is not reused.  After 50 unique client
    or server connections (bindings only), you will run out of connections to
    create.  A linked list or similar structure could fix this problem.  A loop
    to recycle items in the array since they are marked (-1) would work as well.

    Note this does not prevent a server process from getting 50+ connections
    to it since accept() calls are not affected.
*/

#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h> /* bzero */
#include <errno.h>
#include <netdb.h>

#include "tcp.h"

#define BACKLOG    20
#define TCPMAXCONS 50

/* storage for each unique socket binding or outbound connection */
typedef struct {
    int sockfd;  /* socket file descriptor */
    struct sockaddr_in sa; /* socket info structure */
    char * address; /* original address as a string */
    int port;       /* port to connect to or to bind on */
    bool server;    /* is this a server binding or client connection? */
} tcpconn;

/* module level globals */
static tcpconn cons[TCPMAXCONS]; /* socketdesc table.. each is a connection */
static int ncons;  /* number of connections used, an index into cons */


/*
    inittcp

    Sets up the tcp socket library.  Must be called 1 time before
    using any other functions.  (may be used to setup a linked list
    or other structure in the future to replace cons array!)

    PRECONDITIONS:
    none.

    POSTCONDITIONS:
    tcp sockets can be created.  
*/
void inittcp( void )
{
    ncons = -1;
}


/*
    opentcp

    Creates a new TCP connection for client or server communications.

    Must be called after inittcp has been called once and before any other
    function in the tcp sockets lib.

    PRECONDITIONS:
    Need to create a tcp connection.
    @param server   should this be a server socket?
    @param address  an ip address, DNS name or hostname (www.yahoo.com, etc)
                    for server use, must be localhost, 0.0.0.0, an interface
                    on the server to bind to like say 192.168.0.1 or a hostname
                    for said interface.
    @param port     the port to connect to or bind to (client/server)

    POSTCONDITIONS:
    @returns a "socket descriptor" for the newly created connection.

    A socket descriptor is a index value that represents a connection, thus
    allowing any app that uses this library to create multiple concurrent 
    connections.  (Not thread safe though)  socket descriptor was invented for 
    this library.  Esseintally its similar to the idea of a file descriptor
    for the open() system call.  Each one corresponds to a table in this file.
*/
socketdesc opentcp( bool server, char * address, int port )
{
    struct hostent *he; /* for DNS/hostname/ip lookups */
    int addresslen;     /* length of address string param */

    if ( ncons < TCPMAXCONS -1 )
        ncons++; /* increment the table cons for each connection */
    else
        return ETCPMAXCONS;  /* error to many connections */

    addresslen = strlen( address );

    cons[ncons].server = server; /* hostname or ip as stated above */
    cons[ncons].port = port;     /* port to bind to or connect to */

    /* allocate space to store the original address before doing a lookup */
    if ((cons[ncons].address = (char *) malloc( addresslen+1 * sizeof(char) )) == NULL)
        return ETCPGENERIC;  /* could not allocate memory :( */
    
    strncpy( cons[ncons].address, address, addresslen );

    /* create an endpoint for communication */
    if ((cons[ncons].sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return ETCPGENERIC;
    }

    /* clear socket param struct */
    bzero(&(cons[ncons].sa), sizeof cons[ncons].sa);  

    /* setup some defaults for a TCP ip4 connection with a BIG ENDIAN safe
       port number */
    cons[ncons].sa.sin_family = AF_INET;
    cons[ncons].sa.sin_port = htons(cons[ncons].port);

    /* lookup the hostname or DNS name (or set the ip address) */
    if ((he = gethostbyname(cons[ncons].address)) == NULL) {
        herror(cons[ncons].address);
        return ETCPBADADDRESS;
    }

    /* copy the ip found or listed in BIG ENDIAN SAFE byte order */
    bcopy(he->h_addr_list[0],&(cons[ncons].sa).sin_addr, he->h_length);

    /* for servers, we need to bind and listen for later use */
    if ( server == true )
    {
        /* bind the socket for use */
        if (bind( cons[ncons].sockfd, 
                  (struct sockaddr *)&(cons[ncons].sa), 
                  sizeof cons[ncons].sa) < 0) 
        {
            perror("bind");
            return ETCPBINDFAIL;
        }

        /* listen for incoming connections. */
        if (listen( cons[ncons].sockfd, BACKLOG) < 0)
        {
            perror("listen");
            return ETCPLISTENFAIL;
        }

    }
    else
    {
        /* must be client! connect but dont do anything else :) */
        if (connect( cons[ncons].sockfd, 
                     (struct sockaddr *)&(cons[ncons].sa), 
                     sizeof cons[ncons].sa) < 0) 
        {
            perror("connect");
            return ETCPCONNFAIL;
        }
    }

    return ncons;  /* socketdesc.. aka the index in the interal 
                      array*/
}


/*
    closetcp
  
    close the socket descriptor and socket connection.  Clean up after
    ourselves.

    PRECONDITIONS:
    @param sd  Socket descriptor to close

    POSTCONDITIONS:
    sd is closed and marked dead in the cons table.  

*/
void closetcp( socketdesc sd )
{
    /* disconnect socket */
    (void) close(cons[sd].sockfd);

    /* as a hint this is dead.  Maybe we can recycle them? */
    cons[sd].sockfd = -1; 

    /* free the address memory storage */
    (void) free(cons[ncons].address);
}


/*
    readntcp

    Reads the specified number of bytes into a character array and 
    returns a pointer to the space.

    YOU MUST FREE THE MEMORY POINTED TO AFTER YOU ARE DONE WITH IT.

    Only valid for client connections.

    PRECONDITIONS:
    Client connection
    @param sd    a socket descriptor to use
    @param size  number of bytes to read

    POSTCONDITIONS:
    @returns a character pointer to malloc'd memory of length size+1
             i.e. null terminated string.  

    NULL returned on any error or if EOF is hit.
*/   
char * readntcp( socketdesc sd, int size )
{
    char * buf;     /* read buffer.. the size of the returned string */
    ssize_t bytes;  /* bytes read.. could be up to size in length */

    if ( size < 1 )
        return NULL;

    if ( (buf = (char *) malloc( (size+1) * sizeof(char) ) ) == NULL )
        return NULL;

    if ( cons[sd].server == false )
    {
        if ((bytes = read(cons[sd].sockfd, buf, size)) < 1)
            return NULL;
    }
    else
    {
        return NULL;
    }

    buf[bytes] = '\0';  /* set last byte to null terminated */

    return buf;
}


/*
    readtcp

    Reads into a character array and returns a pointer to the space.

    YOU MUST FREE THE MEMORY POINTED TO AFTER YOU ARE DONE WITH IT.

    Only valid for client connections.

    PRECONDITIONS:
    Client connection
    @param sd    a socket descriptor to use

    POSTCONDITIONS:
    @returns a character pointer to malloc'd memory of length BUFSIZE+1
             i.e. null terminated string.  

    NULL returned on any error or EOF hit.
*/  
char * readtcp( socketdesc sd )
{
    return readntcp( sd, BUFSIZ );
}


/*
    tcpclientfd

    Retrieves a file descriptor to directly read/write or use c streams
    on a client socket connection.  This is an alternative to the built-in
    readtcp, writetcp and related functions for client connections in this
    library.  

    VALID FOR CLIENTS ONLY.

    PRECONDITIONS:
    client connection
    @param sd   A socket descriptor to use

    POSTCONDITIONS:
    @returns  a file descriptor to the socket.  Use closetcp() to clear the
    file descriptor instead of a close() call when you are done.  It frees
    up other resources as well.

    Returns ETCPGENERIC on any error
*/
int tcpclientfd( socketdesc sd )
{
    if ( cons[sd].server == false )
        return cons[sd].sockfd;
    else
        return ETCPGENERIC;
}


/*
    tcpserverclient

    Used to read/write information to 1 client connected at a time.

    For server mode only!

    PRECONDITIONS:
    a listening socket was created with this library i.e. server mode is
    active.
    @param sd   a socket descriptor

    POSTCONDITIONS:
    @returns a file descriptor to the client connection.  Please close this
             with close() system call when done.

    on error, ETCPACCEPTFAIL is returned.
*/
int tcpserverclient( socketdesc sd )
{
     int clientfd; /* fd representing the first socket in the backlog queue */
     socklen_t b; /* size of sa struct so we can pass it as a mem addr */

     b = (socklen_t) sizeof cons[sd].sa;

     if ((clientfd = accept( cons[sd].sockfd, 
                            (struct sockaddr *) &(cons[sd].sa), &b)) < 0) 
     {
         perror("accept");
         return ETCPACCEPTFAIL;
     }

    return clientfd;
}


/*
    writetcp

    Used to write information over a connection established for a client.

    FOR CLIENT USE ONLY.

    PRECONDITONS:
    a client connection is established and waiting.
    @param sd  socket descriptor to use
    @param str a string to write to the server we are connected to.

    POSTCONDITIONS:
    server is sent the string minus the null term character. :)
    @returns number of bytes written
*/
int writetcp( socketdesc sd, char * str )
{
    return writentcp( sd, str, strlen(str) );
}


/*
    writentcp

    Used to write the specified number of bytes over a connection established
    for a client.

    FOR CLIENT USE ONLY.

    PRECONDITIONS:
    a client connection is established and waiting
    @param sd   socket descriptor
    @param str  a string to write
    @param len  the length of the str data to write out

    POSTCONDITIONS:
    @returns number of bytes written
*/
int writentcp( socketdesc sd, char * str, int len )
{
    if (cons[sd].server == false)
        return write( cons[sd].sockfd, str, len );
    else
        return ETCPGENERIC;
}


/*
    tcpputs

    Used to write a c style string terminated by a newline character to the 
    client.  Similar to the C puts function.

    FOR CLIENT USE ONLY.

    PRECONDITIONS:
    a client connection is established and waiting
    @param sd   socket descriptor
    @param str  a string to write

    POSTCONDITIONS:
    @returns number of bytes written
*/
int tcpputs( socketdesc sd, char * str )
{
    int written = 0;  /* number of bytes written */

    if ((written = writetcp( sd, str )) == ETCPGENERIC)
        return ETCPGENERIC;
    else
      written = written + writetcp( sd, "\n" );

    return written;
}
