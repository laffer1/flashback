/* $Id: fbHttpResponse.cpp,v 1.5 2008/03/21 04:06:05 laffer1 Exp $ */
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

#include "fbHttpServer.h"
#include "fbHttpResponse.h"
#include "fbSocket.h"

fbHttpResponse:: fbHttpResponse(fbData * _data, fbClient * _client): fbThread(_data), data(_data),   client(_client), running(false)
{
}

fbHttpResponse::~fbHttpResponse()
{
    if (running)
        shutdown();
}

void fbHttpResponse::startup()
{
    if (running)
        return;

    start();
}

void fbHttpResponse::shutdown()
{
    if (!running)
        return;

    stop();
    running = false; // stop it gracefully
}

void fbHttpResponse::run()
{
     // we should probably check this during the request  while(!isStopping())
     char *path;
     path = client->getPath();

     if ( strcmp(path, "/") == 0 )
        index();
    else if ( strcmp(path, "/index") == 0 )
        index();
    else
        notfound();

    // we're mallocing this elsewhere.
    if ( path != NULL )
       free( path );

    delete client;

   shutdown();  // clean up 
}

void fbHttpResponse::index()
{

}

void fbHttpResponse::notfound()
{
    string r;  // response for client

    status( "404", "Not Found" );
   // TODO: Date  header
    header( "Server", SERVERID );
    header( "Connection", "close");
    header( "Content-Type", "text/html; charset=iso-8859-1" );
    header( "Content-Language", "en-US" );
    // TODO: Expires header

    r.append("\r\n"); // extra to start response as required per spec
    r.append( "<html>\n<head>\n\t<title>404 Not Found</title>\n</head>\n");
    r.append("<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on the server.</p>\n");
    r.append("</body>\n</html>\n");

    client->write(r);
}

// ok so it's invalid for .9... 
void fbHttpResponse::status( string code, string msg )
{
    string r;
    r.append( "HTTP/1.0 " );
    r.append( code );
    r.append( " " );
    r.append( msg );
    r.append("\r\n");
    client->write(r);
}

void fbHttpResponse::header( string name, string value )
{
    string r;
    r.append(name);
    r.append(": ");
    r.append(value);
    r.append("\r\n");
    client->write(r);
}

