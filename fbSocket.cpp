/* $Id: fbSocket.cpp,v 1.17 2008/03/29 17:25:19 laffer1 Exp $ */

/*-
 * Copyright (C) 2008 Lucas Holt. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <cassert>
#include "fbSocket.h"

/**
 * fbSocket implementation
 * @author Lucas Holt
 * @date March 7, 2008
 */

fbSocket::fbSocket(fbData* _data, string addr, int port ):data(_data)
{
    char *a;
    setBindAddress( addr );
    setBindPort( port );

    data->debug(NONE, "fbSocket.this");

    inittcp(); /* setup socket library */

    a = strdup(getBindAddress().c_str());
    if ((sd = opentcp( true, a, getBindPort() )) < 0 ) {
        data->err(NONE, "Could not bind to TCP port");
    }

    free(a);
    data->debug(NONE, "fbSocket.this : Must have a binding on TCP port");
}

fbSocket::~fbSocket()
{
    closetcp( sd );
    data->debug(NONE, "fbSocket.~this");
}

string fbSocket::getBindAddress()
{
    return bindAddress;
}

void fbSocket::setBindAddress( string addr )
{
    data->debug(NONE, "fbSocket.setBindAddress()");
    bindAddress = addr;
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
    data->debug(NONE, "fbSocket.nextClient()");
    return new fbClient(data, tcpserverclient( sd ) ); /* has to be deleted by the caller */
}

