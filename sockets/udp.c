/*
author: Lucas Holt & Rupali Y Mahajan
date due: April 21, 2005
assignment: Documentation Project, TCP/UDP Sockets
class: cs224
class time: 11am MWF

An library to abstract the C sockets interface from the programmer.  This
file implements the UDP protocol.    

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

#include "udp.h"

#define UDPMAXCONS 50

typedef struct {
    int sockfd;  /* socket file descriptor */
    struct sockaddr_in sa; /* socket info structure */
    char * address; /* original address as a string */
    int port;       /* port to connect to or to bind on */
    bool server;    /* is this a server binding or client connection? */
} udpconn;

/* module level globals */
static udpconn cons[UDPMAXCONS]; /* socketdesc table.. each is a connection */
static int ncons;  /* number of connections used, an index into cons */

/*
    initudp

    Sets up the udp socket library.  Must be called 1 time before
    using any other functions.  (may be used to setup a linked list
    or other structure in the future to replace cons array!)

    PRECONDITIONS:
    none.

    POSTCONDITIONS:
    udp sockets can be created.  
*/
void initudp( void )
{
    ncons = -1;
}


/*
    openudp

    Creates a new UDP connection for client or server communications.

    Must be called after initudp has been called once and before any other
    function in the udp sockets lib.

    PRECONDITIONS:
    Need to create a udp connection.
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
socketdesc openudp( bool server, char * address, int port )
{
    struct hostent *he; /* for DNS/hostname/ip lookups */
    int addresslen;     /* length of address string param */

    if ( ncons < UDPMAXCONS -1 )
        ncons++; /* increment the table cons for each connection */
    else
        return EUDPMAXCONS;  /* error to many connections */

    addresslen = strlen( address );

    cons[ncons].server = server; /* hostname or ip as stated above */
    cons[ncons].port = port;     /* port to bind to or connect to */

    /* allocate space to store the original address before doing a lookup */
    if ((cons[ncons].address = (char *) malloc( addresslen+1 * sizeof(char) )) == NULL)
        return EUDPGENERIC;  /* could not allocate memory :( */
    
    strncpy( cons[ncons].address, address, addresslen );

    /* create an endpoint for communication */
    if ((cons[ncons].sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return EUDPGENERIC;
    }

    /* clear socket param struct */
    bzero(&(cons[ncons].sa), sizeof cons[ncons].sa);  

    /* setup some defaults for a TCP ip4 connection with a BIG ENDIAN safe
       port number */
    cons[ncons].sa.sin_family = AF_INET;
    cons[ncons].sa.sin_port = htons(cons[ncons].port);

    /* lookup the hostname or DNS name (or set the ip address) */
    if ((he = gethostbyname(cons[ncons].address)) == NULL) 
    {
        herror(cons[ncons].address);
        return EUDPBADADDRESS;
    }

    /* copy the ip found or listed in BIG ENDIAN SAFE byte order */
    bcopy(he->h_addr_list[0],&(cons[ncons].sa).sin_addr, he->h_length);

    /* for servers, we need to bind and listen for later use */
    if ( server == TRUE )
    {
        /* bind the socket for use */
        if (bind( cons[ncons].sockfd, 
                  (struct sockaddr *)&(cons[ncons].sa), 
                  sizeof cons[ncons].sa) < 0) 
        {
            perror("bind");
            return EUDPBINDFAIL;
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
            return EUDPCONNFAIL;
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
void closeudp( socketdesc sd )
{
    /* disconnect socket */
    /* udp is odd. you have to actually do another connect call
       with a special flag to disconnect it! */
    cons[sd].sa.sin_family = AF_UNSPEC; /* since we are now disconnecting..*/
    connect( cons[sd].sockfd, 
                     (struct sockaddr *)&(cons[ncons].sa), 
                     sizeof cons[sd].sa);

    /* as a hint this is dead.  Maybe we can recycle them? */
    cons[sd].sockfd = -1; 

    /* free the address memory storage */
    (void) free(cons[ncons].address);
}


/*
    sendudp

    Sends a udp (datagram) to the target

    PRECONDITIONS:
    @param sd   socket descriptor we've opened up already (see openudp)
    @param msg  the message we want to send.. must be the whole thing
    @param len  The length of the message to send.
    @param address The address to send the message to (hostname or ip) 

    Its legal to have an empty message in UDP.

    POSTCONDITIONS:
    @returns the number of bytes sent.
 */
long int sendudp( socketdesc sd, char * msg, int len, char * address )
{
    struct sockaddr_in sa; /* its destination address structure */
    struct hostent *he;    /* for DNS/hostname/ip lookups */

    /* need an address to send to */
    if ( address == NULL )
        return EUDPGENERIC;

    /* clear socket param struct */
    bzero(&sa, sizeof sa); 
 
    /* set defaults */
    sa.sin_family = AF_INET;
    sa.sin_port = htons(cons[sd].port);

    /* lookup the hostname or DNS name (or set the ip address) */
    if ((he = gethostbyname(address)) == NULL) 
    {
        herror(address);
        return EUDPBADADDRESS;
    }

    /* copy the ip found or listed in BIG ENDIAN SAFE byte order */
    bcopy(he->h_addr_list[0],&sa.sin_addr, he->h_length);

    return (long) sendto( cons[sd].sockfd, msg, len, 0, (struct sockaddr *) &sa, sizeof sa );
}


/*
    recvudp

    Recieves a udp (datagram).  This blocks inside waiting for the packet.

    PRECONDITIONS:
    @param sd   socket descriptor we've opened up already (see openudp)
    @param msg  a buffer to store the message
    @param len  the maximum size the message can be (buffer size)
    @param address (optional, can be NULL) the address it came from.   

    POSTCONDITIONS:
    @returns the number of bytes sent.
    msg contains the message received which can be null by spec definition.

    bugs: address lookups have not been fully tested. 
 */
long int recvudp( socketdesc sd, char * msg, int len, char * address )
{
    struct sockaddr_in from;  /* what address did we get this from? */
    socklen_t fromlen;        /* the length of the from address struct */
    ssize_t n;                /* number of bytes received */
    struct hostent *he;       /* for DNS/hostname/ip lookups */

    n = recvfrom( cons[sd].sockfd, (void *) msg, len, 0, 
                  (struct sockaddr *) &from, (socklen_t *) &fromlen );

    /* address can be null in some cases :) */
    if ( address == NULL )
        return (long) n;

    /* lookup the dns name (PTR record or host file) for the ip we have recieved from */
    if ((he = gethostbyaddr((char *) &(from.sin_addr), sizeof (from.sin_addr), AF_INET)) == NULL) 
    {
        address = NULL;
        return (long) n;
    }

    /* copy the address as a string so we can use it for future calls to
       sendudp */
    address = (char *) malloc( sizeof(char) * (he->h_length + 1) );

    /* copy the address into the output variable */
    if ( address )
        strncpy( address, he->h_addr_list[0], (he->h_length) );

    return (long) n;
}
