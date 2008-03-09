/* $Id: fbSocket.cpp,v 1.8 2008/03/09 08:49:47 laffer1 Exp $ */

#include <cassert>
#include "fbSocket.h"

/**
 * fbSocket implementation
 * @author Lucas Holt
 * @date March 7, 2008
 */

fbSocket::fbSocket( /*const fbErrorLogger &err,*/ char *addr, int port )
{
    //log = err;
    setBindAddress( addr );
    setBindPort( port );

    inittcp(); /* setup socket library */

    if (sd = opentcp( true, getBindAddress(), getBindPort() ) < 0 )
    {
        //log.print(ERR, UNKNOWN, "Could not bind to TCP port");
	exit(1);
    }
}

fbSocket::~fbSocket()
{
    closetcp( sd );
}

char * fbSocket::getBindAddress()
{
    return bindAddress;
}

void fbSocket::setBindAddress( char *addr )
{
    strncpy( bindAddress, addr, 16 );
    bindAddress[15] = '\0';
}

int fbSocket::getBindPort()
{
    return bindPort;
}

void fbSocket::setBindPort( int port )
{
    assert(port > 0);
    bindPort = port;
}

fbClient * fbSocket::nextClient()
{ 
    return new fbClient( tcpserverclient( sd ) );
}

