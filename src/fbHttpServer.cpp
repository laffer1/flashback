/* $Id: fbHttpServer.cpp,v 1.11 2008/03/27 17:48:14 wyverex Exp $ */
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

fbHttpServer:: fbHttpServer(fbData* _data):fbThread(_data), data(_data), servsock(NULL), running(false)
{
	data->debug(NONE, "fbHttpServer.this");
}

fbHttpServer::~fbHttpServer()
{
    if (servsock != NULL)
        delete servsock;

    if (running)
        shutdown();

    data->debug(NONE, "fbHttpServer.~this");
}

void fbHttpServer::startup()
{
    data->debug(NONE, "fbHttpServer.startup");

    if (running)
        return;

    string bindhost = "127.0.0.1";

    servsock = new fbSocket(data, (char *) bindhost.c_str(), 8080 ); // TODO: Use the settings object to set these.
    start();
}

void fbHttpServer::shutdown()
{
    if (!running)
        return;

    stop();
    running = false; // stop it gracefully
}

void fbHttpServer::run()
{
    
    fbClient *client;
    fbHttpResponse *resp;

    data->debug(NONE, "fbHttpServer.run");

    while(!isStopping())
    {
	client = servsock->nextClient();
        client->parseHeaders();
        resp = new fbHttpResponse( data, client ); /*  This class calles delete on itself */
        resp->startup();
    }
}

