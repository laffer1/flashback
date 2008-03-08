/* $Id: fbSocket.cpp,v 1.5 2008/03/08 05:29:28 laffer1 Exp $ */

#include <cassert>
#include "fbSocket.h"

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
