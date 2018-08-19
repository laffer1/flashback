/* $Id: fbHttpServer.cpp,v 1.18 2009/05/17 02:46:43 laffer1 Exp $ */
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
#include "fbSocket.h"


/**
*	fbHttpServer
*	Default constructor
*	@note Initilize all member vars
*/
fbHttpServer:: fbHttpServer(fbData* _data):fbThread(_data), data(_data), servsock(NULL)
{
    data->debug(NONE, "fbHttpServer.this");
}


/**
*	fbHttpServer
*	Default  Destructor
*	@note Initilize all member vars
*/
fbHttpServer::~fbHttpServer()
{
    if (servsock != NULL)
        delete servsock;

    if (isRunning())
        shutdown();

    data->debug(NONE, "fbHttpServer.~this");
}


/**
*	startup
*       Begin web server operation
*/
void fbHttpServer::startup()
{
    data->debug(NONE, "fbHttpServer.startup");

    if (isRunning())
        return;

    servsock = new fbSocket(data, (char *) data->getWebServerAddr().c_str(), data->getWebServerPort() );

    data->debug(NONE, "fbHttpServer.startup socket created?");
    start();
}


/**
*	shutdown
*	halt the webserver
*/
void fbHttpServer::shutdown()
{
    if (isRunning())
     	stop();
}


/**
*	run
*       Handle all client requests.
*	@note main loop spawns threads for each request.
*/
void fbHttpServer::run()
{
    fbClient *client;              // a new client to deal with
    fbHttpResponse *resp;  // a new HttpResponse to tell the client what to do.

    data->debug(NONE, "fbHttpServer.run");

    while(!isStopping())
    {
        data->debug(NONE, "fbHttpServer.run() wait for client connection");
        client = servsock->nextClient();
        client->parseHeaders();
        data->debug(NONE, "fbHttpServer.run() Create response object");
        resp = new fbHttpResponse( data, client ); /*  This class calles delete on itself */
        resp->startup();
    }
}

