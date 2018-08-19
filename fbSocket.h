/* $Id: fbSocket.h,v 1.13 2008/03/29 06:24:39 laffer1 Exp $ */

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

#ifndef FBSOCKET_H
#define FBSOCKET_H

#include "global.h"
#include "sockets/tcp.h"
#include "fbData.h"
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
    fbSocket(fbData* _data, string addr, int port );   /// Default Constructor
    ~fbSocket();  /// Destructor

    string getBindAddress();
    void setBindAddress( string addr );

    int getBindPort();
    void setBindPort( int port );

    fbClient *nextClient();

protected:
    fbData* data;
    int bindPort;
    string bindAddress; /* ipv4 only */
    socketdesc sd;
};

#endif
