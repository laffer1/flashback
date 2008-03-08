#include <cassert>
#include "fbSocket.h"

fbSocket::fbSocket( char *addr, int port )
{
    inittcp(); /* setup socket library */

}

void fbSocket::setBindAddress( char *addr )
{
    strncpy( bindAddress, addr, 16 );
    bindAddress[15] = '\0';
}

void fbSocket::setBindPort( int port )
{
    assert(port > 0);
    bindPort = port;
}
