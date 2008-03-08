/* $Id: fbSocket.h,v 1.6 2008/03/08 05:29:28 laffer1 Exp $ */
#ifndef FBSOCKET_H
#define FBSOCKET_H

#include "global.h"
#include "sockets/tcp.h"

/**
 * fbSocket
 * Server Socket interface
 * @author Lucas Holt
 * @date March 7, 2008
 */
class fbSocket
{
public:
    fbSocket( /*const fbErrorLogger &err, */ char *addr, int port );   /// Default Constructor
    ~fbSocket();  /// Destructor

    char *getBindAddress();
    void setBindAddress( char *addr );

    int getBindPort();
    void setBindPort( int port );

protected:
    //fbErrorLogger log;
    int bindPort;
    char bindAddress[16]; /* ipv4 only */
    socketdesc sd;
};

#endif
