/* $Id: fbHttpResponse.cpp,v 1.9 2008/03/29 01:05:49 laffer1 Exp $ */
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

#include <sys/param.h>
#include <stdlib.h>

#include "fbHttpServer.h"
#include "fbHttpResponse.h"
#include "fbSocket.h"

fbHttpResponse:: fbHttpResponse(fbData * _data, fbClient * _client): fbThread(_data), data(_data),   client(_client), running(false)
{
    data->debug(NONE, "fbHttpResponse.this");
}

fbHttpResponse::~fbHttpResponse()
{
    if (running)
        shutdown();

    data->debug(NONE, "fbHttpResponse.~this");
}

void fbHttpResponse::startup()
{
    data->debug(NONE, "fbHttpResponse.startup");

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

    delete this;
}

void fbHttpResponse::run()
{
     // we should probably check this during the request  while(!isStopping())
    char *path;
    path = client->getPath();

    data->debug(NONE, "fbHttpResponse.run");

    sendfile(path);

/*
    if ( strcmp(path, "/") == 0 )
        index();
    else if ( strcmp(path, "/index") == 0 )
        index();
    else
        notfound();
*/

    // we're mallocing this elsewhere.
    if ( path != NULL )
       free( path );

    delete client;

   shutdown();  // clean up 
}

void fbHttpResponse::sendfile( const char * path )
{
    FILE *fp;
    string realp = "/usr/local/share/flashback/www/";
    char tmp[513];
    char resolved[PATH_MAX];
    char *towrite;

    data->debug(NONE, "fbHttpResponse.sendfile");

    if (path != NULL)
        realp.append(path);
    else
        return; // TODO: Error logging and 404?

    realpath(realp.c_str(), resolved);

    if (*resolved)
    {
        status( "200", "OK" );
    }
    else
    {
        notfound();
        return;
    }
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", "text/html; charset=iso-8859-1" );
    header( "Content-Language", "en-US" );
    client->write("\r\n\r\n"); // end header section

    if ( (fp = fopen( resolved, "r" ) ) == NULL ) {
        // TODO HANDLE ERROR
        data->err(NONE, "fbHttpResponse: Unable to open file");
        data->err(NONE, resolved);
        return;
    }
        
    while ( !feof( fp ) )
    {
        towrite = fgets(tmp, 512, fp);
        if (towrite)
            client->write(towrite);
    }
    data->debug(NONE, "fbHttpResponse.sendfile() done writing file");
}

void fbHttpResponse::notfound()
{
    string r;  // response for client
    char *path;

    data->debug(NONE, "fbHttpServer.notfound");

    path = client->getPath();

    status( "404", "Not Found" );
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", "text/html; charset=iso-8859-1" );
    header( "Content-Language", "en-US" );
    // TODO: Expires header

    r.append("\r\n"); // extra to start response as required per spec
    r.append( "<html>\n<head>\n\t<title>404 Not Found</title>\n</head>\n");
    r.append("<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on the server.</p>\n");
    r.append("<p>Invalid path: ");
    r.append(path);
    r.append("</p>\n<hr><p>");
    r.append(SERVERID);
    r.append("</p>\n</body>\n</html>\n");

    client->write(r);

    if (path != NULL)
        free( path );
}

// ok so it's invalid for .9... 
void fbHttpResponse::status( string code, string msg )
{
    string r;
    r.append( "HTTP/1.0 " );
    r.append( code );
    r.append( " " );
    r.append( msg );
    r.append( "\r\n" );
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

void fbHttpResponse::headdate()
{
    struct tm *tm;
    time_t now;
    char date[50];
    string r;

    now = time( 0 );
    tm = gmtime( &now ); /* HTTP 1.1 spec rev 06 sez GMT only */
    strftime( date, 50, "%a, %d %b %Y %H:%M:%S GMT", tm );
    r.append(date);
    header( "Date", r );
}

