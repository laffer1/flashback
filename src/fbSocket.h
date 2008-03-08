#ifndef FBSOCKET_H
#define FBSOCKET_H

#include "global.h"
#include "sockets/tcp.h"

/**
 * Server Socket interface
 */
class fbSocket
{
public:
    fbSocket( char *addr, int port );   /// Default Constructor
    ~fbSocket();  /// Destructor

    void setBindAddress( char *addr );
    void setBindPort( int port );

protected:
    int bindPort;
    char bindAddress[16]; /* ipv4 only */
};

#endif
