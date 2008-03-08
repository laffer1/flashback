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
    fbSocket( const fbErrorLogger &err, char *addr, int port );   /// Default Constructor
    ~fbSocket();  /// Destructor

    char *getBindAddress();
    void setBindAddress( char *addr );

    int getBindPort();
    void setBindPort( int port );

protected:
    fbErrorLogger log;
    int bindPort;
    char bindAddress[16]; /* ipv4 only */
    socketdesc sd;
};

#endif
