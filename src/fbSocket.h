/* $Id: fbSocket.h,v 1.9 2008/03/09 08:49:47 laffer1 Exp $ */
#ifndef FBSOCKET_H
#define FBSOCKET_H

#include "global.h"
#include "sockets/tcp.h"
#include "fbClient.h"

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

    fbClient *nextClient();

protected:
    //fbErrorLogger log;
    int bindPort;
    char bindAddress[16]; /* ipv4 only */
    socketdesc sd;
};

#endif
